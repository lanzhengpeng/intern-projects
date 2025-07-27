### 【3161】list all bodies features 列出所有体的特征

#### 代码

```cpp
    /*HEAD LIST_ALL_BODIES_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，我理解UF_print_syslog是在V18版本中新增的。 */  
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
    static void list_feats(tag_t item)  
    {  
        int  
            i,  
            count,  
            subtype,  
            type;  
        uf_list_p_t  
            list;  
        char  
            msg[133],  
            *feat_name,  
            *feat_type;  
        tag_t  
            feat;  
        UF_CALL( UF_OBJ_ask_type_and_subtype(item, &type, &subtype));  
        switch (subtype)  
        {  
            case UF_solid_body_subtype:  
                UF_CALL( UF_MODL_ask_body_feats( item, &list));  
                printf("Body");  
                break;  
            case UF_solid_face_subtype:  
                UF_CALL( UF_MODL_ask_face_feats( item, &list));  
                printf("Face");  
                break;  
            case UF_solid_edge_subtype:  
                UF_CALL( UF_MODL_ask_edge_feats( item, &list));  
                printf("Edge");  
                break;  
            default:  
                printf("Huh?");  
                break;  
        }  
        UF_CALL( UF_MODL_ask_list_count( list, &count));  
        printf(" has %d features\n", count);  
        for (i = 0; i < count; i++)  
        {  
            UF_CALL( UF_MODL_ask_list_item( list, i, &feat));  
            UF_CALL( UF_MODL_ask_feat_type( feat, &feat_type));  
            UF_CALL( UF_MODL_ask_feat_name( feat, &feat_name));  
            sprintf(msg, "  %d.  %s - %s\n", i+1, feat_name, feat_type);  
            if (!UF_UI_open_listing_window())  
                UF_UI_write_listing_window(msg);  
            UF_free( feat_name);  
            UF_free( feat_type);  
        }  
        UF_CALL(UF_MODL_delete_list(&list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((body = ask_next_body(part, body)) != NULL_TAG) list_feats(body);  
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

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能是遍历当前显示部件中的所有实体，并输出每个实体的特征信息。具体来说：
>
> 1. 代码首先包含了一些必要的头文件，并定义了一个错误报告函数report_error。
> 2. ask_next_body函数用于遍历部件中的实体，返回下一个实体的tag。
> 3. list_feats函数用于输出一个实体的所有特征信息，包括特征名称和类型。
> 4. do_it函数遍历所有实体，并对每个实体调用list_feats函数。
> 5. ufusr函数是程序的入口，初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. ufusr_ask_unload函数用于在卸载用户函数时返回立即卸载标志。
>
> 总的来说，这段代码通过遍历实体并输出特征信息，提供了在NX中查看和了解实体特征的一种方法。
>
