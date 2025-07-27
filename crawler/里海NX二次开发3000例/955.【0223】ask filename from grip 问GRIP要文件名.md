### 【0223】ask filename from grip 问GRIP要文件名

#### 代码

```cpp
    /*HEAD ASK_FILENAME_FROM_GRIP CCC UFUN */  
    /*  This is a generic UG/Open API (Ufunc) program which can be called whenever  
        interactive file selection is required in a GRIP program.  It will put up  
        a file selection dialog.  You can specify the "Cue", dialog title, initial  
        directory, and filter.  The selected filename is returned to the GRIP  
        program.  If the user selects Cancel the filename will come back &nulstr.  
        Since the directory and filter can be changed by the user, they are also  
        returned to the calling GRIP program, so if necessary the program can be  
        called again and the dialog will start where it was left off.  
        Here is sample GRIP code demonstating how to use the program:  
            string/ cue(80), title(80), dir_filter(132), filename(132)  
            number/ resp  
            grargs/ cue, title, dir_filter, filename, resp  
            cue = 'Choose text file to open'  
            title = 'Open Text File'  
            dir_filter = '*.txt'  $$ path defaults to current directory  
    a10:    XSPAWN/ ufun, '/path/to/ask_filename_from_grip.sl'  
            ifthen/ resp == 2  
                fetch/ txt, 2, filename, iferr, x10:  
                fprint/ 2  
            endif  
            halt  
    x10:    messg/'Cannot open text file', 'Please select another'  
            jump/ a10:  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数。在翻译中，只回答UF_print_syslog是V18版本新增的函数，不需要添加其他内容。 */  
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
    static void do_it(void)  
    {  
        int  
            i_resp;  
        double  
            d_resp;  
        char  
            cue[81],  
            title[81],  
            dir_filter[133],  
            filename[133];  
        UF_args_t  
            grip_args[5];  
        grip_args[0].type = UF_TYPE_CHAR;  
        grip_args[0].length = 0;  
        grip_args[0].address = cue;  
        grip_args[1].type = UF_TYPE_CHAR;  
        grip_args[1].length = 0;  
        grip_args[1].address = title;  
        grip_args[2].type = UF_TYPE_CHAR;  
        grip_args[2].length = 0;  
        grip_args[2].address = dir_filter;  
        grip_args[3].type = UF_TYPE_CHAR;  
        grip_args[3].length = 0;  
        grip_args[3].address = filename;  
        grip_args[4].type = UF_TYPE_DOUBLE;  
        grip_args[4].length = 0;  
        grip_args[4].address = &d_resp;  
        UF_CALL(UF_ask_grip_args(5, grip_args));  
        UF_CALL(UF_UI_create_filebox(cue, title, dir_filter, filename,  
            filename, &i_resp));  
        d_resp = i_resp;  
        UF_CALL(UF_set_grip_args(5, grip_args));  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open的UFUNC程序，主要功能是用于在NX GRIP程序中调用时，弹出文件选择对话框，以实现文件的选择。
>
> 主要特点如下：
>
> 1. 使用UF_ask_grip_args函数获取GRIP传递的参数，包括提示信息、对话框标题、初始目录和文件过滤等。
> 2. 使用UF_UI_create_filebox函数创建文件选择对话框，并显示给用户，让用户选择文件。
> 3. 用户选择文件后，将文件名通过UF_set_grip_args函数返回给GRIP程序。
> 4. 使用UF_print_syslog函数在系统日志中打印错误信息。
> 5. 提供了ufusr和ufusr_ask_unload两个接口供GRIP程序调用。
>
> 该程序通过UFUNC方式实现了GRIP程序与用户交互选择文件的功能，具有很好的通用性。
>
