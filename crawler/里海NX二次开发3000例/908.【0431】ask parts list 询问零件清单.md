### 【0431】ask parts list 询问零件清单

#### 代码

```cpp
    /*HEAD ASK_PARTS_LIST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  plist:  UF_print_syslog is new in V18 译:根据您提供的plist信息，翻译结果如下：

UF_print_syslog 是V18版本新增的功能。 */  
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
    static int ask_object_layer(tag_t object)  
    {  
        logical  
            is_displayable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable &&  
            !UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props)))  
                return disp_props.layer;  
        else  
            return -1;  
    }  
    static tag_t ask_parts_list_tag(tag_t part, tag_t plist)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &plist)) && (plist != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(plist, &type, &subtype));  
            if ((subtype == UF_parts_list_subtype) &&  
                (ask_object_layer(plist) > 0)) return plist;  
        }  
        return NULL_TAG;  
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
        tag_t  
            plist = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        if ((plist = ask_parts_list_tag(part, plist)) != NULL_TAG)  
        {  
            WRITE_D(plist);  
        }  
        else  
        {  
            WRITE("There is no Parts List\n");  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
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
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是从NX部件中查询并打印部件清单(plist)的标签(tag)。具体代码逻辑如下：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了ask_object_layer函数，用于查询对象的可显示属性和图层。
> 3. 定义了ask_parts_list_tag函数，用于在指定部件中遍历对象，找到类型为部件清单的对象，并返回其标签。
> 4. 定义了WRITE和WRITE_D宏，用于在列表窗口打印文本和整数。
> 5. 定义了do_it函数，用于打开当前显示的部件，查询部件清单对象，并在列表窗口打印其标签。
> 6. 定义了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 7. 定义了check_load_status函数，用于检查部件加载状态，并在控制台打印加载失败的信息。
> 8. 主函数中，首先初始化NX，然后通过uc4624函数获取命令行传入的部件文件名，逐个打开部件，调用do_it函数处理，并在控制台打印已处理的部件数量。最后终止NX。
>
> 总体来说，这段代码实现了查询并打印NX部件中部件清单标签的功能，代码逻辑清晰，函数定义合理。
>
