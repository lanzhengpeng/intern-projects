### 【2525】save current part as new nxmanager master 将当前部件另存为新NX Manager主部件

#### 代码

```cpp
    /*HEAD SAVE_CURRENT_PART_AS_NEW_NXMANAGER_MASTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。

翻译：UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            mod;  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            new_part[MAX_FSPEC_SIZE + 1],  
            part_num[UF_UGMGR_PARTNO_SIZE+1],  
            part_rev[UF_UGMGR_PARTREV_SIZE+1],  
            save_as[MAX_FSPEC_SIZE+1];  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
        UF_CALL(UF_UGMGR_assign_part_number("", "", part_num, &mod));  
        UF_CALL(UF_UGMGR_assign_part_rev(part_num, "", part_rev, &mod));  
        value.value.string = part_num;  
        UF_CALL(UF_ATTR_assign(part, "DB_PART_NO", value));  
        UF_CALL(UF_ATTR_assign(part, "DB_PART_NAME", value));  
        UF_CALL(UF_ATTR_assign(part, "DB_PART_DESC", value));  
        UF_CALL(UF_UGMGR_encode_part_filename(part_num, part_rev, "", "", save_as));  
        UF_CALL(UF_PART_save_as(save_as));  
        UF_CALL(UF_PART_name_for_display(save_as, new_part));  
        WRITE_S(new_part);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是将当前工作部件保存为一个新的NX部件，并设置一些属性。以下是代码的主要功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个写入字符串到列表窗口的函数write_string_to_listing_window，用于显示信息。
> 3. do_it函数是主要功能实现：获取当前工作部件生成新的部件编号和版本号设置部件的DB_PART_NO、DB_PART_NAME、DB_PART_DESC属性编码生成新的部件文件名保存当前部件为新的NX部件显示新部件的名称
> 4. 获取当前工作部件
> 5. 生成新的部件编号和版本号
> 6. 设置部件的DB_PART_NO、DB_PART_NAME、DB_PART_DESC属性
> 7. 编码生成新的部件文件名
> 8. 保存当前部件为新的NX部件
> 9. 显示新部件的名称
> 10. ufusr函数是NX的入口函数，在这里调用do_it函数。
> 11. ufusr_ask_unload函数用于卸载该二次开发程序。
>
> 通过这段代码，用户可以快速将当前工作部件保存为一个新的NX部件，并设置一些属性，实现了部件的快速保存功能。
>
