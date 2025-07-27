### 【1983】report all udos linked to selected object 报告所有与选定对象关联的用户定义对象

#### 代码

```cpp
    /*HEAD REPORT_ALL_UDOS_LINKED_TO_SELECTED_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_udobj.h>  
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
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        ECHO(msg);  
    }  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    static void report_udo_data(tag_t udo)  
    {  
        int  
            ii,  
            status,  
            subtype,  
            type;  
        char  
            *class_name;  
        UF_UDOBJ_all_data_t  
            data;  
        char  
            out[MAX_LINE_SIZE+1];  
        WRITE_D(udo);  
        if (UF_CALL(UF_UDOBJ_ask_udo_class_name(udo, &class_name))) return;  
        WRITE_S(class_name);  
        UF_free(class_name);  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        WRITE_D(data.udo_status);  
        WRITE_D(data.num_ints);  
        WRITEnD(data.num_ints, data.ints);  
        WRITE_D(data.num_doubles);  
        WRITEnF(data.num_doubles, data.doubles);  
        WRITE_D(data.num_strings);  
        for (ii = 0; ii < data.num_strings; ii++)  
        {  
            WRITE_S(data.strings[ii]);  
        }  
        WRITE_D(data.num_links);  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            if (data.link_defs[ii].assoc_ug_tag != NULL_TAG)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(  
                    data.link_defs[ii].assoc_ug_tag, &type, &subtype));  
                status = UF_OBJ_ask_status(data.link_defs[ii].assoc_ug_tag);  
                sprintf(out, "\t\t%d. type %d to %d (%d/%d %d) status %d\n", ii+1,  
                    data.link_defs[ii].link_type, data.link_defs[ii].assoc_ug_tag,  
                    type, subtype, status, data.link_defs[ii].object_status);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
            else  
            {  
                sprintf(out, "\t\t%d. type %d to NULL_TAG\n", ii+1,  
                    data.link_defs[ii].link_type);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
        }  
        WRITE_D(data.num_lengths);  
        WRITEnF(data.num_lengths, data.lengths);  
        WRITE_D(data.num_areas);  
        WRITEnF(data.num_areas, data.areas);  
        WRITE_D(data.num_volumes);  
        WRITEnF(data.num_volumes, data.volumes);  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_links;  
        tag_t  
            object;  
        UF_UDOBJ_link_t  
            *udo_links;  
        while ((object = select_anything("Report linked UDOs")) != NULL_TAG)  
        {  
            UF_CALL(UF_UDOBJ_ask_udo_links_to_obj(object, &n_links, &udo_links));  
            WRITE_D(n_links);  
            for (ii = 0; ii < n_links; ii++)  
                report_udo_data(udo_links[ii].assoc_ug_tag);  
            if (n_links > 0) UF_free(udo_links);  
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

```

#### 代码解析

> 这段NX二次开发代码实现了选择一个对象，并报告与此对象关联的所有用户定义对象(UDO)的功能。主要功能包括：
>
> 1. 使用UF_UI模块选择任意对象，包括面和边。
> 2. 使用UF_UDOBJ模块查询与选定对象关联的所有UDO。
> 3. 对于每个UDO，报告其类名、状态、整型数据、浮点数据、字符串数据、关联对象类型和状态、长度、面积、体积等数据。
> 4. 使用UF_UI模块将UDO数据输出到日志窗口。
> 5. 提供了卸载函数，使加载项可以立即卸载。
>
> 代码的关键点包括：
>
> 综上所述，这段代码实现了选择对象并查询与之关联的所有UDO的功能，并提供了完善的错误处理和日志输出，是一个典型的NX二次开发示例。
>
