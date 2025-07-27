### 【1501】list all daxis layer 列出所有基准轴图层

#### 代码

```cpp
    /*HEAD LIST_ALL_DAXIS_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_layer.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的注释，可以翻译为：

注意：UF_print_syslog是在V18版本中新增的，请仅回答翻译，不要添加其他无关内容。 */  
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
    static tag_t ask_next_daxis(tag_t part, tag_t daxis)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_datum_axis_type, &daxis));  
        return (daxis);  
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
    static void do_it(void)  
    {  
        tag_t  
            daxis = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((daxis = ask_next_daxis(part, daxis)) != NULL_TAG)  
            report_object_layer(daxis);  
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

> 这段代码是一个NX的二次开发程序，其主要功能是遍历当前显示部件中的所有基准轴，并报告每个基准轴所在的层。
>
> 主要步骤包括：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义了一个错误报告函数report_error，用于在调用NX Open API失败时打印错误信息。
> 3. 定义了一个函数ask_next_daxis，用于遍历部件中的下一个基准轴。
> 4. 定义了一个函数report_object_layer，用于报告一个基准轴所在的层。
> 5. 定义了一个函数do_it，用于遍历所有基准轴并报告它们所在的层。
> 6. 定义了ufusr函数，作为程序的入口。在ufusr中，首先调用UF_initialize进行初始化，然后调用do_it执行主逻辑，最后调用UF_terminate终止。
> 7. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示程序可以被立即卸载。
>
> 总体来说，这段代码实现了遍历当前显示部件的基准轴，并报告它们所在层的功能。
>
