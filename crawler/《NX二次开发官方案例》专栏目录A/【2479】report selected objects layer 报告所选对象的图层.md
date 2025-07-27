### 【2479】report selected objects layer 报告所选对象的图层

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_OBJECTS_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_layer.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的函数，专门用于在系统日志中打印日志信息。 */  
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
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_object_layer(tag_t object)  
    {  
        int  
            component_layer,  
            new_layer,  
            object_layer,  
            prototype_layer,  
            subtype,  
            type;  
        tag_t  
            component,  
            prototype;  
        UF_OBJ_disp_props_t  
            disp_props;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        object_layer = disp_props.layer;  
        sprintf(msg, "object_layer = %d", object_layer);  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        if (UF_ASSEM_is_occurrence(object) && (type != UF_component_type))  
        {  
            component = UF_ASSEM_ask_part_occurrence(object);  
            UF_CALL(UF_OBJ_ask_display_properties(component, &disp_props));  
            component_layer = disp_props.layer;  
            prototype = UF_ASSEM_ask_prototype_of_occ(object);  
            UF_CALL(UF_OBJ_ask_display_properties(prototype, &disp_props));  
            prototype_layer = disp_props.layer;  
            if (object_layer == component_layer)  
            {  
                UF_CALL(UF_DISP_set_display(FALSE));  
                new_layer = object_layer + 1;  
                if (new_layer > UF_LAYER_MAX_LAYER) new_layer = 1;  
                UF_CALL(UF_OBJ_set_layer(component, new_layer));  
                UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
                if (disp_props.layer == new_layer)  
                    strcat(msg, ", inherited from component\n");  
                else  
                    strcat(msg, ", set on component member\n");  
                UF_CALL(UF_OBJ_set_layer(component, component_layer));  
                UF_CALL(UF_DISP_set_display(TRUE));  
            }  
            else if (object_layer == prototype_layer)  
            {  
                UF_CALL(UF_DISP_set_display(FALSE));  
                new_layer = object_layer + 1;  
                if (new_layer > UF_LAYER_MAX_LAYER) new_layer = 1;  
                UF_CALL(UF_OBJ_set_layer(prototype, new_layer));  
                UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
                if (disp_props.layer == new_layer)  
                    strcat(msg, ", inherited from original object\n");  
                else  
                    strcat(msg, ", set on component member\n");  
                UF_CALL(UF_OBJ_set_layer(prototype, prototype_layer));  
                UF_CALL(UF_DISP_set_display(TRUE));  
            }  
            else  
                strcat(msg, ", set on component member\n");  
        }  
        else  
            strcat(msg, "\n");  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_an_object("Report layer")) != NULL_TAG)  
            report_object_layer(object);  
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

> 这段代码是用于NX二次开发的UF函数，主要功能是选择一个对象，并报告该对象的图层信息。
>
> 主要步骤包括：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时报告错误信息。
> 2. 定义了一个选择对象函数select_an_object，用于通过对话框选择一个对象，并返回其tag。
> 3. 定义了一个报告对象图层函数report_object_layer，用于报告指定对象的图层信息。
> 4. 主函数do_it循环调用select_an_object选择对象，然后调用report_object_layer报告图层信息。
> 5. ufusr函数是NX二次开发的主要入口函数，在这里调用do_it执行主要功能。
> 6. ufusr_ask_unload函数用于卸载当前UF函数。
>
> 该代码的主要功能是让用户选择一个对象，然后报告该对象所在的图层信息，包括对象自身的图层、组件的图层、原型的图层等。通过判断对象是否是组件实例以及图层信息，可以确定图层是从组件继承还是设置在组件成员上。
>
