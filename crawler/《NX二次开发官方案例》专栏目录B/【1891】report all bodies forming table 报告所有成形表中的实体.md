### 【1891】report all bodies forming table 报告所有成形表中的实体

#### 代码

```cpp
    /*HEAD REPORT_ALL_BODIES_FORMING_TABLE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_smd.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            count,  
            index;  
        tag_t  
            body = NULL_TAG,  
            frmtbl,  
            part = UF_PART_ask_display_part();  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (UF_CALL(UF_SMD_initialize())) return;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL(UF_SMD_ask_form_tbl(body, &frmtbl));  
            if (frmtbl != NULL_TAG)  
            {  
                UF_CALL(UF_SMD_ask_form_tbl_count(frmtbl, &count));  
                UF_CALL(UF_SMD_ask_form_tbl_current_seq(frmtbl, &index));  
                sprintf(msg,  
                    "Body = %d\nForming Table = %d\nCount = %d\nCurrent Seq = %d",  
                    body, frmtbl, count, index);  
                uc1601(msg, TRUE);  
            }  
            else  
            {  
                sprintf(msg, "Body = %d\nForming Table = %d", body, frmtbl);  
                uc1601(msg, TRUE);  
            }  
        }  
        UF_SMD_terminate();  
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

> 这段代码是用于NX二次开发的，其主要功能是遍历当前零件中的所有实体体，并获取每个实体的成形表信息，然后打印出相关信息。
>
> 主要步骤包括：
>
> 1. 初始化NX API
> 2. 获取当前显示的零件
> 3. 循环遍历零件中的所有实体体，并获取每个实体的成形表
> 4. 如果成形表存在，则打印出实体的tag、成形表的tag、成形表中的项目数量和当前序列号
> 5. 如果成形表不存在，则只打印实体的tag和成形表的tag
> 6. 完成后终止NX API
>
> 代码中定义了两个辅助函数：
>
> 主函数是do_it，负责实现上述功能。ufusr函数是二次开发的入口函数，调用do_it函数来实现功能。ufusr_ask_unload函数用于设置二次开发模块的卸载方式。
>
> 总体而言，这段代码实现了遍历实体体并获取成形表信息的功能，为NX的二次开发提供了示例。
>
