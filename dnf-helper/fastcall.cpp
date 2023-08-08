#include "common.h"
#include "fastcall.h"
#include "helper.h"
#include <iostream>


FastCall fastCall;


static int g_hook_interface;
static int g_call_max_len ;
static int g_RSP ;
static int g_timeout_call_settings ;
static ULONG64 g_allocate_space ;
static int g_last_space ;
static int g_hook_address ;
static int g_old_data ;
static int g_old_data_save ;
static int g_transit_framework_memory ;
static ULONG64 g_execute_func_code ;
static ULONG64 g_execute_func_result ;
static ULONG64 g_execute_func_control ;
static ULONG64 g_execute_func_refresh_time ;
static ULONG64 g_execute_func_last_time ;
static ULONG64 g_execute_func_data ;
static int g_hook_framework ;

// 初始化
void FastCall::InitCode()
{
    g_hook_interface = 1;
    g_call_max_len = 6666;
    g_RSP = 584;
    g_allocate_space = (ULONG64)rw.ApplyMemory(4096 * 1024);
    g_timeout_call_settings = 1000 * 60;
    g_last_space = g_allocate_space;

    vector<byte> code = { 72, 137, 116, 36, 8, 72, 137, 124, 36, 16, 65, 86, 72, 131, 236, 32, 72, 190, 0, 0, 0, 64, 1, 0, 0, 0,
    72, 191, 118, 11, 204, 63, 1, 0, 0, 0, 73, 190, 126, 11, 204, 63, 1, 0, 0, 0, 255, 214, 72, 163, 142,
    11, 204, 63, 1, 0, 0, 0, 131, 63, 1, 117, 57, 73, 199, 6, 0, 0, 0, 0, 72, 199, 7, 2, 0, 0, 0, 255, 214,
    72, 163, 134, 11, 204, 63, 1, 0, 0, 0, 65, 86, 87, 83, 86, 72, 184, 0, 0, 0, 80, 1, 0, 0, 0, 255, 208,
    94, 91, 95, 65, 94, 73, 137, 6, 199, 7, 0, 0, 0, 0, 72, 139, 116, 36, 48, 72, 139, 124, 36, 56, 72, 131,
    196, 32, 65, 94, 195 };

    g_execute_func_code = AllocateSpace(sizeof(code));
    g_execute_func_data = AllocateSpace(g_call_max_len);
    g_execute_func_control = AllocateSpace(8);
    g_execute_func_refresh_time = AllocateSpace(8);
    g_execute_func_result = AllocateSpace(8);
    g_execute_func_last_time = AllocateSpace(8);

    rw.WriteBytes(g_execute_func_code, code);
    rw.WriteLong(g_execute_func_code + 0x10 + 2, rw.ReadLong(GameTimeGetTime));
    rw.WriteLong(g_execute_func_code + 0x1A + 2, g_execute_func_control);
    rw.WriteLong(g_execute_func_code + 0x24 + 2, g_execute_func_result);
    rw.WriteLong(g_execute_func_code + 0x30 + 2, g_execute_func_refresh_time);
    rw.WriteLong(g_execute_func_code + 0x4F + 2, g_execute_func_last_time);
    rw.WriteLong(g_execute_func_code + 0x5E + 2, g_execute_func_data);

    code = { 72, 137, 92, 36, 8, 72, 137, 116, 36, 16, 87, 72, 131, 236, 32 };
    code = code + makeByteArray({ 72, 184 }) + IntToBytes(g_execute_func_code, 8), makeByteArray({ 255, 208 });
    code = code + makeByteArray({ 72, 139, 92, 36, 48, 72, 139, 116, 36, 56, 72, 131, 196, 32, 95, 195 });
    g_transit_framework_memory = AllocateSpace(sizeof(code));
    rw.WriteBytes(g_transit_framework_memory, code);
    InitHookType(g_hook_interface);

    code = { 80, 83, 81, 82, 87, 86, 85, 65, 80, 65, 81, 65, 82, 65, 83, 65, 84, 65, 85, 65, 86, 65, 87, 156, 72,
    131, 236, 40 };
    code = code + makeByteArray({ 72, 184 }) + IntToBytes(g_transit_framework_memory, 8) + makeByteArray({255, 208});
    code = code + makeByteArray({ 72, 131, 196, 40, 157, 65, 95, 65, 94, 65, 93, 65, 92, 65, 91, 65, 90, 65, 89, 65, 88,
        93, 94, 95, 90, 89, 91, 88 });
    code = code + makeByteArray({ 255, 37, 0, 0, 0, 0 }) + IntToBytes(g_old_data, 8);

    g_hook_framework = AllocateSpace(sizeof(code));

    rw.WriteBytes(g_hook_framework, code);
    rw.WriteLong(g_old_data_save, g_old_data);
    rw.WriteLong(g_hook_address, g_hook_framework);
}
// 释放内存
void FastCall::FreeCode()
{
    rw.WriteLong(g_hook_address, g_old_data);
    rw.WriteBytes(g_transit_framework_memory, GetEmptyBytes(g_last_space - g_allocate_space));
}
// 分配空间
ULONG64 FastCall::AllocateSpace(int len)
{
    ULONG64 result = g_last_space;
    g_last_space = g_last_space + len;
    return result;
}

void FastCall::InitHookType(int interfaceSelect)
{
    g_old_data_save = AllocateSpace(8);
    if(interfaceSelect == 1){
        ULONG64 hook_address = rw.ReadLong(TranslateMessage1);
        hook_address = hook_address + rw.ReadInt(hook_address + 2) + 6;
        g_hook_address = hook_address;
    }
    if (rw.ReadLong(g_old_data_save) == 0) {
        g_old_data = rw.ReadLong(g_hook_address);
    }
    else{
        g_old_data = rw.ReadLong(g_old_data_save);
    }

}
// 调用等待
void FastCall::CallWait()
{
    while (rw.ReadInt(g_execute_func_control) == 1) {
        Sleep(1);
    }

    while (rw.ReadInt(g_execute_func_control) == 2) {
        int refresh_time = rw.ReadInt(g_execute_func_refresh_time) - rw.ReadInt(
            g_execute_func_last_time);
        if (g_timeout_call_settings != 0 and refresh_time > g_timeout_call_settings) {
            break;
        }
        Sleep(1);
    }
           
}
// 自动堆栈
ULONG64 FastCall::CallFunctionAutoFindStack(vector<byte> callData, int rsp)
{
    if (rsp == NULL) {
        rsp = g_RSP;
    }
     
    if (callData.size() > 0 && callData[callData.size() - 1] == 195) {
        callData[callData.size() - 1] = 144;
    }
        

    callData = makeByteArray({ 72, 129, 236 }) + IntToBytes(rsp, 4) +
        callData + makeByteArray({ 72, 129, 196 }) + IntToBytes(rsp, 4);

    return MemoryCompileCall(callData);

}

ULONG64 FastCall::MemoryCompileCall(vector<byte> callData)
{
    CallWait();
    callData = callData + makeByteArray({ 195 });
    if (sizeof(callData) > g_call_max_len) {
        cout << "调用数过长" << endl;
        return 0;
    }

    rw.WriteBytes(g_execute_func_data, callData);
    rw.WriteInt(g_execute_func_control, 1);
    CallWait();
    rw.WriteBytes(g_execute_func_data, GetEmptyBytes(sizeof(callData)));
    return rw.ReadLong(g_execute_func_result);
}

ULONG64 FastCall::Call(ULONG64 address, vector<ULONG64> data)
{
    if (sizeof(data) > 16) {
        return 0;
    }
    vector<UINT> instruction_array ={ 47432, 47688, 47177, 47433 };

    vector<byte> code = {};
    for (int i = 0; i < sizeof(data); i++) {
        if(i < 4){
            code = code + IntToBytes(instruction_array[i], 2);
            code = code + IntToBytes(data[i], 8);
        }
        else {
            code = code + makeByteArray({ 72, 184 }) + IntToBytes(data[i], 8);
            code = code + makeByteArray({ 72, 137, 132, 36 }) + IntToBytes(i * 8, 4);
        }
    }
    code = code + makeByteArray({ 72, 184 }) + IntToBytes(address, 8) + makeByteArray({ 255, 208 });
    int rsp;
    if (sizeof(data) < 4) {
        rsp = 4 * 8 + 8;
    }else{
        rsp = sizeof(data) * 8 + 8;
    }
    if(rsp / 8 % 2 == 0){
        rsp = rsp + 8;
    }
    code = makeByteArray({ 72, 129, 236 }) + IntToBytes(rsp, 4) 
        + code 
        + makeByteArray({ 72, 129, 196 }) + IntToBytes(rsp, 4);

    return MemoryCompileCall(code);
}
