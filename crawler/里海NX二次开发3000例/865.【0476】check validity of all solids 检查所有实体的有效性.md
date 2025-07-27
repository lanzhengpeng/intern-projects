### 【0476】check validity of all solids 检查所有实体的有效性

#### 代码

```cpp
    /*HEAD CHECK_VALIDITY_OF_ALL_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog is new in V18，请只回答翻译，不要添加其他内容。

UF_print_syslog在V18中是新增的。 */  
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
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static logical do_it(void)  
    {  
        int  
            cnt = 0,  
            irc,  
            one = 1,  
            v_flag[1];  
        tag_t  
            body[1] = { NULL_TAG },  
            part = UF_PART_ask_display_part();  
        while ((body[0] = ask_next_body(part, body[0])) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_validate_body(1, body, v_flag));  
            if (v_flag[0] == 2)  
            {  
                cnt++;  
                UF_CALL(UF_OBJ_set_name(body[0], "INVALID"));  
            }  
        }  
        if (cnt)  
        {  
            printf("  %d invalid bodies found\n", cnt);  
            return TRUE;  
        }  
        else return FALSE;  
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
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            int  
                cnt = 0,  
                ii;  
            tag_t  
                part;  
            UF_PART_load_status_t  
                status;  
            for (ii=1; ii<argc; ii++)  
            {  
                printf("%d.  %s\n", ++cnt, argv[ii]);  
                if (UF_CALL(UF_PART_open(argv[ii], &part, &status))  
                    || status.n_parts > 0) report_load_status(&status);  
                else  
                {  
                    if (status.n_parts > 0) report_load_status(&status);  
                    if (do_it()) UF_CALL(UF_PART_save());  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. 初始化和终止：使用UF_initialize()和UF_terminate()进行NX的初始化和终止。
> 2. 错误报告：定义了一个报告错误的函数report_error()，用于在调用UF函数出错时打印错误信息。
> 3. 循环遍历零件中的实体：定义了一个函数ask_next_body()，用于循环遍历零件中的实体，直到遍历完所有实体。
> 4. 检查实体有效性：定义了一个函数do_it()，用于检查零件中的每个实体是否有效，并将无效实体重命名为"INVALID"。
> 5. 主函数：在主函数中，接收命令行传入的零件文件名，依次打开每个零件，检查实体有效性，并在找到无效实体时保存零件文件。
> 6. 加载状态报告：定义了一个函数report_load_status()，用于报告零件加载的状态。
> 7. 用户函数：定义了一个ufusr()函数，供NX调用，实现零件实体的有效性检查。
> 8. 卸载请求：定义了一个函数ufusr_ask_unload()，请求立即卸载用户函数。
>
> 总体来说，这段代码实现了通过NX二次开发检查零件文件中实体有效性的功能，并提供了友好的错误报告和加载状态报告。
>
