### 【0781】create new nxmanager part of custom type 创建新的NX管理器自定义类型零件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <uf_part.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
    static void do_it(void)  
    {  
        logical  
            mod;  
        tag_t  
            part;  
        char  
            encoded[MAX_FSPEC_SIZE+1],  
            part_num[UF_UGMGR_PARTNO_SIZE+1],  
            part_rev[UF_UGMGR_PARTREV_SIZE+1],  
            t_pn[UF_UGMGR_PARTNO_SIZE+1],  
            t_rev[UF_UGMGR_PARTREV_SIZE+1],  
            t_coded[MAX_FSPEC_SIZE+1],  
            type[MAX_FSPEC_SIZE+1] = { "" };  
        UF_CALL(UF_UGMGR_assign_part_number("", "", part_num, &mod));  
        UF_CALL(UF_UGMGR_assign_part_rev(part_num, "", part_rev, &mod));  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "", "", encoded));  
        if ((prompt_for_text("Enter template part ItemID", t_pn))  
            && (prompt_for_text("Enter template part Rev", t_rev))  
            && (prompt_for_text("Enter type for new part", type)))  
        {  
            UF_CALL(UF_UGMGR_encode_part_filename(t_pn, t_rev, "master", "",  
                t_coded));  
            /* The type specified here is ignored if the specified "template" is  
             * not a part which NX knows is a template.  In that case the part  
             * will inherit the type from the specified part.  If that is a  
             * problem, then use the newer methods as recorded in a journal.  
             */  
            UF_CALL(UF_UGMGR_new_part_from_template(encoded, type, t_coded, &part));  
            UF_CALL(UF_PART_save());  
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

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的NX二次开发头文件。
> 2. 定义了一个错误报告函数report_error，用于输出函数调用失败的错误信息。
> 3. 定义了一个提示输入文本的函数prompt_for_text，用于在对话框中提示用户输入文本。
> 4. 定义了主函数do_it，主要流程如下：获取当前打开零件的ItemID和Rev，并编码成文件名。提示用户输入模板零件的ItemID、Rev和新零件的类型。根据模板零件的ItemID、Rev编码成文件名。调用UF_UGMGR_new_part_from_template从模板创建新零件。保存当前零件。
> 5. 获取当前打开零件的ItemID和Rev，并编码成文件名。
> 6. 提示用户输入模板零件的ItemID、Rev和新零件的类型。
> 7. 根据模板零件的ItemID、Rev编码成文件名。
> 8. 调用UF_UGMGR_new_part_from_template从模板创建新零件。
> 9. 保存当前零件。
> 10. 定义了ufusr函数，这是NX二次开发的入口函数，在这里调用初始化函数，执行do_it函数，然后终止。
> 11. 定义了ufusr_ask_unload函数，返回立即卸载二次开发模块。
> 12. 整体流程是：首先获取当前零件信息，然后提示用户输入新零件的信息，接着从模板创建新零件，最后保存当前零件。
>
