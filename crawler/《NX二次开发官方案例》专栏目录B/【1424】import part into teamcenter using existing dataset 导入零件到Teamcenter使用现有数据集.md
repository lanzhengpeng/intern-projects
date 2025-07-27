### 【1424】import part into teamcenter using existing dataset 导入零件到Teamcenter使用现有数据集

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clone.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void build_temp_filespec(char *fspec, int ftype, char *new_spec);  
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
    static void write_string_to_listing_window(char *title, const char *string)  
        {  
            if (string != NULL)  
                ECHO("%s = \"%s\"\n", title, string);  
            else  
                ECHO("%s = NULL\n", title);  
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
    void build_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4575(tmp_dir, ftype, fspec, new_spec));  
    }  
    static void do_it(void)  
    {   
        char inputPart[UF_ATTR_MAX_STRING_LEN+1] = { "" };  
        char dBasePart[UF_ATTR_MAX_STRING_LEN+1] = { "" };  
        char clone_log[MAX_FSPEC_SIZE+1];  
        char encoded_name [ MAX_FSPEC_BUFSIZE ] ;  
        const char * part_number = { dBasePart };  
        const char * part_revision = { "A" };  
        const char * part_file_type = { "" };  
        const char * part_file_name = { "" };  
        UF_CLONE_naming_failures_t name_fails;  
        // TODO: add your code here  
        uc1601("Please pay attention to the cue line for input requirements. \nThe first prompt please enter the operating system file name of the NX native part.\nThe second prompt please enter the desired database Item name.", 1);  
        prompt_for_text("Enter NX native fileName.", inputPart);  
        prompt_for_text("Enter Database Item name.", dBasePart);  
        ECHO("Native NX filename entered: %s\n", inputPart);  
        ECHO("Target database item name entered: %s\n", dBasePart);  
        build_temp_filespec("Clone_Existing_Import", 47, clone_log);  
        UF_CALL(UF_CLONE_initialise(UF_CLONE_import_operation));  
        UF_CALL(UF_CLONE_set_def_action(UF_CLONE_use_existing));  
        UF_CALL(UF_CLONE_set_def_assoc_file_copy(FALSE));  
        UF_CALL(UF_CLONE_set_logfile(clone_log));  
        WRITE_S(clone_log);  
        uc4561(clone_log, 0);  
        UF_CALL(UF_CLONE_set_def_item_type("Item"));  
        UF_CALL(UF_CLONE_add_part(inputPart));  
        UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision, part_file_type, part_file_name, encoded_name));  
        WRITE_S(encoded_name);  
        UF_CALL(UF_CLONE_set_naming(inputPart , UF_CLONE_user_name, encoded_name));  
        UF_CALL(UF_CLONE_init_naming_failures(&name_fails));  
        UF_CALL(UF_CLONE_perform_clone(&name_fails));  
        UF_CALL(UF_CLONE_terminate());  
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

> 这是NX二次开发的一个示例代码，主要实现以下功能：
>
> 1. 通过提示用户输入，获取NX原生文件名和数据库项名称。
> 2. 构建克隆操作的日志文件路径。
> 3. 初始化UF_CLONE模块，设置使用现有数据库项的默认行为，不复制关联文件，设置日志文件。
> 4. 将用户输入的原生文件添加到克隆操作中。
> 5. 使用UF_UGMGR模块，根据用户输入的数据库项名称、版本等信息，编码得到数据库文件名。
> 6. 设置克隆操作的命名规则，使用用户输入的数据库文件名。
> 7. 初始化命名失败的标志，并执行克隆操作。
> 8. 终止UF_CLONE模块。
> 9. NX二次开发接口函数ufusr()中，初始化UF模块，执行do_it()函数，并终止UF模块。
> 10. 提供ufusr_ask_unload()函数，让NX在卸载用户函数时立即卸载。
>
> 总体来说，这段代码实现了根据用户输入的原生文件名和数据库项名称，进行克隆操作，生成新的数据库文件，并使用了UF_CLONE和UF_UGMGR两个模块来简化操作。
>
