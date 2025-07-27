### 【2892】simulate info window header from grip 从grip模拟信息窗口标题

#### 代码

```cpp
    /*============================================================================  
    This example works on HP-UX.  
    You may need to change the environment variables for other platforms.  
    ============================================================================*/  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_(X)  UF_UI_write_listing_window(X)  
    static void simulate_info_window_header_from_grip()  
    {  
        int  
            now[2] = { -1, -1 };  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            user[257] = { "" },  
            partname[257] =  { "" },  
            nodename[257] =  { "" },  
            msg[257] = { "" },  
            *envvar = NULL,  
            dstr[21] = { "" },  
            mstr[MAX_LINE_SIZE+1] = { "" },  
            tstr[21] = { "" };  
        if( NULL_TAG != part )  
        {  
            UF_PART_ask_part_name( part, partname );  
        }  
        UF_UI_exit_listing_window();  
        UF_UI_open_listing_window();  
        WRITE_( "============================================================\n");  
        UF_CALL( UF_translate_variable( "USER", &envvar ) );      
        strcpy( user, envvar );  
        sprintf( msg, "Information listing created by :  %s\n", user );  
        WRITE_( msg );  
        UF_CALL(uc4582(now, 2, dstr, tstr));  
        sprintf(mstr, "%s %s", dstr, tstr);  
        sprintf( msg, "Date                           :  %s", mstr );  
        WRITE_( msg );  
        WRITE_( "\n" );  
        sprintf( msg, "Current work part              :  %s\n", partname );  
        WRITE_( msg );  
        UF_CALL( UF_translate_variable( "SESSION_SVR", &envvar ) );  
        strcpy( nodename, envvar );  
        sprintf( msg, "Node name                      :  %s\n", nodename );  
        WRITE_( msg );  
        WRITE_( "============================================================\n" );  
    }

```

#### 代码解析

> 这段NX Open API二次开发代码的主要功能是模拟NX信息窗口的头部。具体来说，代码实现了以下功能：
>
> 1. 获取当前工作部件名称，并输出到信息窗口。
> 2. 获取当前用户名，并输出到信息窗口。
> 3. 获取当前日期和时间，并输出到信息窗口。
> 4. 获取当前会话的服务器节点名，并输出到信息窗口。
> 5. 输出分隔线，以美化信息窗口头部。
>
> 代码的关键点包括：
>
> 1. 使用UF_PART_ask_part_name获取当前工作部件名称。
> 2. 使用UF_translate_variable获取环境变量，如当前用户名和服务器节点名。
> 3. 使用uc4582获取当前日期和时间。
> 4. 使用UF_UI_open_listing_window和UF_UI_write_listing_window输出信息到NX信息窗口。
> 5. 使用sprintf格式化字符串，并输出到信息窗口。
>
> 通过模拟NX信息窗口的头部，这段代码实现了在NX信息窗口中输出自定义信息的功能。
>
