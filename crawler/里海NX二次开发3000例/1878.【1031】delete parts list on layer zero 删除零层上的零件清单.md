### 【1031】delete parts list on layer zero 删除零层上的零件清单

#### 代码

```cpp
    /*HEAD DELETE_PARTS_LIST_ON_LAYER_ZERO CCC UFUN */  
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
        /*  plist:  UF_print_syslog is new in V18 里海译:根据您的要求，关于 "UF_print_syslog is new in V18" 的翻译如下：

"UF_print_syslog 是 V18 的新功能。" */  
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
            if (subtype == UF_parts_list_subtype) return plist;  
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
        int  
            plist_layer=-1;  
        if ((plist = ask_parts_list_tag(part, plist)) != NULL_TAG)  
        {  
            WRITE_D(plist);  
            plist_layer = ask_object_layer(plist);  
            WRITE_D(plist_layer);  
            if( plist_layer == 0 )  
            {  
                WRITE("Deleting Parts List");  
                UF_CALL( UF_OBJ_delete_object(plist) );  
            }  
        }  
        else  
        {  
            WRITE("There is no Parts List\n");  
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

> 这段NX二次开发代码主要用于在NX中删除零件层为0的零件清单。主要功能包括：
>
> 1. 初始化和终止：使用UF_initialize和UF_terminate进行初始化和终止。
> 2. 获取当前显示的零件：通过UF_PART_ask_display_part获取当前显示的零件。
> 3. 查找零件清单：通过循环查找零件的子对象，判断是否为零件清单，如果是则获取其tag。
> 4. 获取零件清单所在的层：通过UF_OBJ_ask_display_properties获取零件清单所在的层。
> 5. 删除层为0的零件清单：如果零件清单所在的层为0，则使用UF_OBJ_delete_object删除该零件清单。
> 6. 交互式加载零件：主函数中可以交互式加载零件，然后删除零件清单。
> 7. 错误处理：使用UF_get_fail_message获取错误消息，并打印错误。
> 8. 日志输出：使用UF_print_syslog和UF_UI_write_listing_window输出日志。
>
> 总的来说，这段代码实现了在NX中根据零件清单所在的层来删除零件清单的功能，同时具有交互式加载零件和错误处理等功能。
>
