### 【2528】save part as and export jt file 保存部件并导出JT文件

#### 代码

```cpp
    /*HEAD SAVE_PART_AS_AND_EXPORT_JT_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新的。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    void export_jt_of_partname(char *fspec)  
    {  
        char  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            pv_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            prog_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir;  
            UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
            UF_CALL(uc4575(base_dir, 100, "pvtrans", pv_dir));  
            UF_CALL(uc4575(pv_dir, 0, "ugtopv", prog_name));  
            sprintf(cmd, "%s %s", prog_name, fspec);  
            uc1601("Running jt Conversion", TRUE);  
            system(cmd);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_ask_part_name(part, part_fspec);  
        if(prompt_for_text("Save As:", part_fspec))  
        {  
            if(!UF_CALL(UF_PART_save_as(part_fspec)))  
            {  
                export_jt_of_partname(part_fspec);  
            }  
        }  
    }  
    /* qq3123197280 */  
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

> 这段代码是NX的二次开发代码，主要功能是将当前显示的零件另存为指定的文件名，并导出为JT格式。代码的主要逻辑如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误代码和错误信息。
> 2. 定义了一个提示输入文本的函数prompt_for_text，用于提示用户输入零件名称。
> 3. 定义了一个导出JT文件的函数export_jt_of_partname，根据零件名称构造命令，并调用系统命令来导出JT文件。
> 4. 主函数do_it中，获取当前显示的零件，并调用prompt_for_text获取用户输入的零件名称。如果用户确认，则先调用UF_PART_save_as将零件另存为指定文件，然后调用export_jt_of_partname导出JT文件。
> 5. ufusr是NX二次开发的入口函数，初始化NX环境后调用do_it执行主要功能，最后终止NX环境。
> 6. ufusr_ask_unload函数返回立即卸载标志，表示二次开发模块不需要在NX中驻留。
>
> 总的来说，这段代码实现了在NX中保存当前显示的零件为指定文件，并将其导出为JT格式的功能，主要使用了UF_PART和UF_UI这两个NX提供的二次开发接口。
>
