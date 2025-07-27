### 【2853】sew all sheets 将所有钣金件缝合在一起

#### 代码

```cpp
    /*HEAD SEW_ALL_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_sheet_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SHEET_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_sheets(tag_t part, tag_t **sheets)  
    {  
        tag_t  
            sheet = NULL_TAG;  
        uf_list_p_t  
            sheet_list;  
        UF_CALL(UF_MODL_create_list(&sheet_list));  
        while ((sheet = ask_next_sheet_body(part, sheet)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(sheet_list, sheet));  
        return (make_an_array(&sheet_list, sheets));  
    }  
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d\n", name,object,type,subtype);  
        }  
        else printf("%s = %d (NULL_TAG)\n", name, object);  
    }  
    static void report_list_contents(char *name, uf_list_p_t list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            item;  
        char  
            item_name[15];  
        UF_CALL(UF_MODL_ask_list_count(list, &n));  
        printf("%s has %d items\n", name, n);  
        for (ii = 0; ii < n; ii++)  
        {  
            sprintf(item_name, "item %d", ii);  
            UF_CALL(UF_MODL_ask_list_item(list, ii, &item));  
            report_tag_type_and_subtype(item_name, item);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n_sheets;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            sew,  
            *sheets;  
        double  
            distol;  
        uf_list_p_t  
            disjoint_list;  
        char  
            msg[133];  
        UF_MODL_ask_distance_tolerance(&distol);  
        n_sheets = ask_all_sheets(part, &sheets);  
        ECHO(n_sheets);  
        if (n_sheets > 1)  
        {  
            sprintf(msg, "Sewing %d sheets...", n_sheets);  
            uc1601(msg, FALSE);  
            UF_CALL(UF_MODL_create_sew(0, 1, sheets, (n_sheets - 1), &sheets[1],  
                distol, 0, &disjoint_list, &sew));  
            report_list_contents("disjoint_list", disjoint_list);  
            UF_CALL(UF_MODL_delete_list(&disjoint_list));  
        }  
        if (n_sheets > 0) UF_free(sheets);  
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

> 这段代码是一个用于NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 实现了ask_next_sheet_body函数，用于遍历零件中的实体，找出所有sheet类型的实体。
> 3. 实现了allocate_memory和make_an_array函数，用于分配内存并从链表复制实体标签到数组。
> 4. 实现了ask_all_sheets函数，用于获取零件中所有sheet实体的标签数组。
> 5. 实现了report_tag_type_and_subtype和report_list_contents函数，用于输出实体标签的类型和子类型，以及链表的内容。
> 6. 主函数do_it中，首先获取当前显示的零件，然后调用ask_all_sheets获取所有sheet实体的标签数组。如果sheet实体数量大于1，则调用UF_MODL_create_sew接口将它们缝合在一起，并输出缝合后的不连续链表内容。
> 7. ufusr函数是NX二次开发程序的入口函数，在这里调用了do_it函数来实现上述功能。
> 8. ufusr_ask_unload函数用于在程序卸载时释放资源。
>
> 这段代码的目的是获取零件中的所有sheet实体，并将它们缝合在一起，输出缝合后的不连续链表，以便进行后续处理。
>
