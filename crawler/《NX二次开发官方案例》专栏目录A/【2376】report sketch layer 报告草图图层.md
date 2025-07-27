### 【2376】report sketch layer 报告草图图层

#### 代码

```cpp
    /*HEAD REPORT_SKETCH_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志消息。 */  
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
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
    tag_t  
        sketch;  
        while ((sketch = select_a_sketch("Select Sketch")) != NULL_TAG)  
        {  
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

> 这段代码是一个用于在NX中显示草图图层信息的二次开发示例代码。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在发生错误时输出错误信息。
> 2. 定义了报告对象图层函数report_object_layer，用于获取对象当前的图层信息，并判断图层是来自组件、原型还是直接设置。
> 3. 定义了选择草图的函数select_a_sketch，用于提示用户选择一个草图对象。
> 4. 主函数do_it用于循环调用select_a_sketch选择草图，然后调用report_object_layer报告草图图层信息。
> 5. ufusr是NX二次开发的入口函数，在初始化NX环境后调用do_it，完成操作后终止NX环境。
> 6. ufusr_ask_unload用于在卸载时立即终止NX环境。
>
> 总的来说，这段代码通过交互方式选择草图对象，并报告其图层信息，展示了NX二次开发的基本流程。
>
