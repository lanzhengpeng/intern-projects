### 【2175】report layer of each sketch 在UGNX的每个草图上报告图层

#### 代码

```cpp
    /*HEAD REPORT_LAYER_OF_EACH_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static void report_object_layer(tag_t object)  
    {  
        int  
            component_layer,  
            new_layer,  
            object_layer,  
            prototype_layer;  
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
        if (UF_ASSEM_is_occurrence(object))  
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
    #define SHOW_NAME(X) report_object_name(#X, X)  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        if (!UF_OBJ_ask_name(object, obj_name))  
            sprintf(msg, "%s (tag %d) is named %s\n", name, object, obj_name);  
        else  
            sprintf(msg, "%s (tag %d) is not named\n", name, object);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            sketch = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            SHOW_NAME(sketch);  
            report_object_layer(sketch);  
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

> 这段代码主要用于报告NX部件中每个草图的所有层。
>
> 主要功能包括：
>
> 1. 初始化NX API并调用主函数do_it()。
> 2. 循环遍历部件中的所有草图，并获取每个草图的tag。
> 3. 对于每个草图，先输出草图的名称，然后调用report_object_layer()函数报告草图的层信息。
> 4. report_object_layer()函数会获取草图的显示属性，并获取其层号。
> 5. 如果草图是组件实例，会进一步获取组件和原型的层号。
> 6. 如果草图层号与组件或原型层号相同，会尝试修改组件或原型的层号，并检查草图层号是否改变，以确定草图层号是否继承自组件或原型。
> 7. 输出草图层号信息，以及层号是否继承自组件或原型。
> 8. 最后，终止NX API。
>
> 这段代码主要用于遍历部件中的草图，检查和报告每个草图的层信息，以确定层号是否继承自组件或原型，并提供了详细的输出报告。
>
