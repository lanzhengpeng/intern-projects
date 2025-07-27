### 【0634】create component of all solids on layer 创建图层上所有实体的组件

#### 代码

```cpp
    /*HEAD CREATE_COMPONENT_OF_ALL_SOLIDS_ON_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog在V18版本中是新增的。 */  
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
    static int ask_all_objects_on_layer(int layer, tag_t **objects)  
    {  
        int  
            type,  
            subtype,   
            status;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&  
            (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            status = UF_OBJ_ask_status(object);  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype) &&  
                ((status == 3) || (UF_OBJ_is_object_a_promotion(object))) &&  
                (!UF_ASSEM_is_occurrence(object)))  
            {  
                UF_CALL(UF_MODL_put_list_item(list, object));  
            }  
        }  
        return make_an_array(&list, objects);  
    }  
    #ifndef UF_LAYER_MAX_LAYER  
        #define UF_LAYER_MAX_LAYER 256  
    #endif  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            layer = 1,  
            units = 2,  
            n_objects;  
        double  
           origin[3] = {0.0, 0.0, 0.0},  
           csys[6] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};  
        char  
            component_name[MAX_FSPEC_SIZE+1];  
        tag_t  
            part =  UF_PART_ask_display_part(),  
            *objects,  
            instance;  
        while (prompt_for_a_layer("Delete objects from", "Layer", &layer))  
        {  
            UF_CALL(UF_LAYER_set_status(layer, UF_LAYER_ACTIVE_LAYER));  
            sprintf(component_name, "layer_%d_component.prt", layer);   
            n_objects = ask_all_objects_on_layer(layer, &objects) ;  
            if (n_objects > 0)  
            {              
                UF_CALL(UF_ASSEM_create_component_part(part, component_name, NULL,   
                    NULL, units, 0, origin, csys, n_objects, objects, &instance));  
            }   
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

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要用于根据指定图层创建组件。以下是代码的主要功能和逻辑：
>
> 1. 引入头文件：代码引入了NX提供的UF开发接口头文件，以便调用NX的函数。
> 2. 错误报告函数：定义了report_error函数，用于在调用UF函数出错时报告错误信息。
> 3. 内存分配函数：定义了allocate_memory函数，用于分配内存。
> 4. 创建数组函数：定义了make_an_array函数，用于从UF列表创建数组。
> 5. 查询图层对象函数：定义了ask_all_objects_on_layer函数，用于查询指定图层上的所有实体对象，并返回对象数组。
> 6. 提示输入图层函数：定义了prompt_for_a_layer函数，用于提示用户输入图层编号，并检查输入是否有效。
> 7. 主体函数：定义了do_it函数，用于循环提示用户输入图层，激活该图层，查询图层上的实体对象，然后调用UF函数创建一个组件。
> 8. UF初始化函数：定义了ufusr函数，用于初始化UF环境，调用do_it函数，然后终止UF环境。
> 9. 卸载函数：定义了ufusr_ask_unload函数，用于设置该例程的卸载方式。
>
> 总体而言，这段代码通过UF提供的API，实现了查询图层对象并创建组件的功能。代码结构清晰，包含了错误处理和内存管理等机制，是一个完整的NX二次开发示例。
>
