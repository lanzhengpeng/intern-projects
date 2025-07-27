### 【2361】report selected route components port positions 报告选定路径组件端口位置

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_ROUTE_COMPONENTS_PORT_POSITIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_route.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 是 V18 版本新增的功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            type,  
            subtype;  
        tag_t  
            obj = NULL_TAG,  
            comp;  
        double  
            position[3];  
        char  
            port_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_ATTR_value_t  
            value;  
        UF_ROUTE_charx_t  
            charx;  
        value.value.string = port_name;  
        while ((comp = select_a_component("Report port positions")) != NULL_TAG)  
        {  
            while ((obj = UF_ASSEM_cycle_ents_in_part_occ(comp, obj)) != NULL_TAG)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
                if (type == UF_route_port_type)  
                {  
                /*  These are two different ways to get the port "name" 里海译:These are two different ways to get the port "name". */  
                    UF_CALL(UF_ATTR_read_value(obj, "UNIQUE_ID", UF_ATTR_string,  
                        &value));  
                    WRITE_S(port_name);  
                    if (!UF_CALL(UF_ROUTE_find_port_charx("UNIQUE_ID",  
                        UF_ROUTE_CHARX_TYPE_STR, obj, &charx)))  
                    {  
                        WRITE("UNIQUE_ID = ");  
                        WRITE(charx.value.s_value);  
                        WRITE("\n");  
                    }  
                    UF_CALL(UF_ROUTE_ask_port_position(obj, position));  
                    display_temporary_point(position);  
                    WRITE3F(position);  
                }  
            }  
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

> 这是NX的二次开发代码，主要功能是选择一个组件，并报告该组件中所有端口的位置。以下是代码的主要功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了一个选择组件的函数select_a_component，该函数使用单选对话框让用户选择一个组件，并返回组件的tag。
> 3. 定义了一个显示临时点的函数display_temporary_point，用于在图形区显示一个临时点。
> 4. 定义了两个写入信息到列表窗口的函数write_string_to_listing_window和write_double_array_to_listing_window。
> 5. 定义了主函数do_it，该函数循环选择组件，然后在组件中循环所有实体，判断是否为端口类型，如果是，则读取端口的名称，并获取端口的位置信息，最后在图形区显示该位置。
> 6. 定义了ufusr函数，该函数初始化NX，调用主函数，然后终止NX。
> 7. 定义了卸载函数ufusr_ask_unload。
>
> 综上所述，这段代码实现了选择组件并报告组件中所有端口位置的功能。
>
