### 【0777】create new item master part in users home folder 在用户主目录中创建新的项目主部件

#### 代码

```cpp
    /*HEAD CREATE_NEW_ITEM_MASTER_PART_IN_USERS_HOME_FOLDER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <uf_attr.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (string != NULL)  
            sprintf(msg, "%s = \"%s\"\n", title, string);  
        else  
            sprintf(msg, "%s = NULL\n", title);  
        ECHO(msg);  
    }  
    static tag_t create_and_save_new_nxmanager_master_part(char *item)  
    {  
        tag_t  
            part;  
        char  
            encoded[MAX_FSPEC_SIZE+1];  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
        value.value.string = item;  
        if (!UF_CALL(UF_UGMGR_encode_part_filename(item, "1", "", "", encoded))  
            && !UF_CALL(UF_PART_new(encoded, 1, &part)))  
        {  
            UF_CALL(UF_ATTR_assign(part, "DB_PART_NAME", value));  
            UF_CALL(UF_ATTR_assign(part, "DB_PART_DESC", value));  
            UF_CALL(UF_PART_save());  
            return part;  
        }  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            mod;  
        UF_UGMGR_tag_t  
            root;  
        char  
             item[UF_UGMGR_PARTNO_SIZE+1];  
        UF_CALL(UF_UGMGR_ask_root_folder(&root));  
        UF_CALL(UF_UGMGR_set_default_folder(root));  
        UF_CALL(UF_UGMGR_assign_part_number("", "", item, &mod));  
        create_and_save_new_nxmanager_master_part(item);  
        WRITE_S(item);  
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

> 这段代码是一个NX Open C++ UG/NX API的二次开发示例，其主要功能是创建一个新的NX部件主模型，并将其保存在当前用户的家目录文件夹中。具体来说：
>
> 1. 首先，代码定义了错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 接着定义了write_string_to_listing_window函数，用于向NX的列表窗口输出信息。
> 3. create_and_save_new_nxmanager_master_part函数负责创建新部件主模型，并设置其名称和描述属性，最后保存到文件。
> 4. do_it函数首先获取当前用户的根文件夹，设置其为默认文件夹，然后生成一个新部件编号，并调用create_and_save_new_nxmanager_master_part函数创建并保存新部件。
> 5. ufusr是NX二次开发的入口函数，其中调用UF_initialize初始化API，执行do_it函数，最后调用UF_terminate终止API。
> 6. ufusr_ask_unload函数用于设置二次开发的卸载模式，这里设置为立即卸载。
>
> 综上所述，这段代码的主要功能是创建一个新部件，并将其保存在用户家目录文件夹中。通过NX的二次开发接口实现了自动化的部件创建流程。
>
