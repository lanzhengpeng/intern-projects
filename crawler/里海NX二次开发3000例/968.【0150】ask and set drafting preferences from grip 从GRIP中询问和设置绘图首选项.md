### 【0150】ask and set drafting preferences from grip 从GRIP中询问和设置绘图首选项

#### 代码

```cpp
    /*HEAD ASK_AND_SET_DRAFTING_PREFERENCES_FROM_GRIP CCC UFUN */  
    /*  This program demonstrates a method to save and reset all drafting  
        preferences from a GRIP program.  The following is example corresponding  
        GRIP code:  
        NUMBER/ mpi(100), mpr(70), which  
        STRING/ radius(26), diameter(26)  
        grargs/ which, mpi, mpr, radius, diameter  
    $$  Load the current drafting preferences  
        which = 1  
        XSPAWN/ UFUN, 'ask_and_set_drafting_preferences_from_grip'  
    $$  Change the general text character size only  
        which = 2  
        old_gts = mpr(45)  
        mpr(45) = 0.25  $$ set general text size  
        XSPAWN/ UFUN, 'ask_and_set_drafting_preferences_from_grip'  
        NOTE/0,0,'This is note uses character size = 0.25'  
    $$  Reset to the original drafting preferences  
        mpr(45) = old_gts  
        which = 2  
        XSPAWN/ UFUN, 'ask_and_set_drafting_preferences_from_grip'  
        HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数，用于在系统日志中打印信息。以下是该函数的简要描述：

UF_print_syslog用于在系统日志中打印信息。该函数在V18版本中新增。

功能：将指定格式的信息打印到系统日志中。

参数：

- fmt: 输出信息的格式字符串，支持%s、%d等格式占位符。

- ...: 可变参数，提供格式字符串中占位符对应的数据。

返回值：无。

注意事项：

1. UF_print_syslog函数仅用于打印信息，不提供日志文件管理功能。

2. 使用UF_print_syslog函数打印的信息将保存在系统日志中，可通过系统日志工具查看。

3. 确保日志文件权限设置正确，以便应用程序可以写入系统日志。

4. 如果应用程序需要自定义日志文件管理，可以考虑使用其他日志库。

5. UF_print_syslog函数的实现可能会根据不同的操作系统平台有所不同。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            mpi[100];  
        double  
            mpd[100],  
            mpr[70],  
            io_flag;  
        char  
            diameter[27],  
            radius[27];  
        UF_args_t  
            args[5];  
        args[0].type = UF_TYPE_DOUBLE;  
        args[0].length = 0;  
        args[0].address = &io_flag;  
        args[1].type = UF_TYPE_DOUBLE_ARRAY;  
        args[1].length = 100;  
        args[1].address = mpd;  
        args[2].type = UF_TYPE_DOUBLE_ARRAY;  
        args[2].length = 70;  
        args[2].address = mpr;  
        args[3].type = UF_TYPE_CHAR;  
        args[3].length = 0;  
        args[3].address = radius;  
        args[4].type = UF_TYPE_CHAR;  
        args[4].length = 0;  
        args[4].address = diameter;  
        if (UF_CALL(UF_ask_grip_args(5, args))) return;  
        switch ((int)io_flag)  
        {  
            case 1:  
                UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
                for (ii = 0; ii < 100; ii++) mpd[ii] = (double) mpi[ii];  
                UF_CALL(UF_set_grip_args(5, args));  
                break;  
            case 2:  
                UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
                break;  
            default:  
                WRITE("Unknown option flag: ");  
                WRITE_D((int)io_flag);  
                break;  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了一个用于保存和恢复NX绘图参数的功能。具体来说，代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时报告错误信息。
> 2. 定义了一个写入列表窗口的函数WRITE，用于在NX的列表窗口中打印信息。
> 3. 定义了一个写入整数到列表窗口的函数WRITE_D，用于在列表窗口中打印整数。
> 4. 定义了一个主函数do_it，用于获取GRIP参数，并根据参数的不同执行不同的操作：如果参数为1，则获取当前的绘图参数并保存到GRIP参数中。如果参数为2，则使用GRIP参数设置当前的绘图参数。如果参数不是1或2，则在列表窗口中打印错误信息。
> 5. 如果参数为1，则获取当前的绘图参数并保存到GRIP参数中。
> 6. 如果参数为2，则使用GRIP参数设置当前的绘图参数。
> 7. 如果参数不是1或2，则在列表窗口中打印错误信息。
> 8. 定义了ufusr函数，作为NX的二次开发入口函数。在ufusr函数中调用do_it函数，以执行绘图参数的保存和恢复操作。
> 9. 定义了ufusr_ask_unload函数，用于卸载二次开发库。
>
> 总的来说，这段代码实现了根据GRIP参数保存和恢复NX绘图参数的功能，通过GRIP参数的传递实现了NX绘图参数的灵活控制。
>
