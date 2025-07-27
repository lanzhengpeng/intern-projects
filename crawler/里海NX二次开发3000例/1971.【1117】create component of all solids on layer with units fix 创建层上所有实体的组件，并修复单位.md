### 【1117】create component of all solids on layer with units fix 创建层上所有实体的组件，并修复单位

#### 代码

```cpp
    /*HEAD CREATE_COMPONENT_OF_ALL_SOLIDS_ON_LAYER_WITH_UNITS_FIX CCC UFUN */  
    /*  
      A very similar program had the units hard-coded to Inches.  
      This uses the actual units of the assembly part.  
    */  
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
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
            units = 0,  
            n_objects;  
        double  
           origin[3] = {0.0, 0.0, 0.0},  
           csys[6] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};  
        char  
            component_name[MAX_FSPEC_SIZE+1];  
        tag_t  
            part =  UF_PART_ask_display_part(),  
            *objects = NULL,  
            instance = NULL_TAG;  
        if(NULL_TAG == part) return;  
        UF_CALL(UF_PART_ask_units(part, &units));  
        while (prompt_for_a_layer("Use bodies on what Layer?", "Layer", &layer))  
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

> 这段代码是NX的二次开发代码，主要功能是在指定图层上创建组件。以下是代码的主要功能和实现方法：
>
> 1. 引入头文件：代码引入了NX提供的各种头文件，包括模型、装配、零件、用户界面、显示和图层等模块的头文件。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 内存分配函数：定义了一个内存分配函数allocate_memory，用于为tag_t数组分配内存。
> 4. 创建数组函数：定义了一个创建数组函数make_an_array，用于将对象列表转换为tag_t数组。
> 5. 获取图层对象函数：定义了一个获取图层对象函数ask_all_objects_on_layer，用于获取指定图层上的所有实体。
> 6. 图层选择函数：定义了一个图层选择函数prompt_for_a_layer，用于提示用户输入要使用的图层号。
> 7. 创建组件函数：定义了一个创建组件函数do_it，用于在指定图层上创建组件。它会循环遍历每个图层，获取该图层上的实体，并将它们创建为一个组件。
> 8. 主函数：定义了主函数ufusr，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在NX退出时卸载该程序。
>
> 整体来看，这段代码实现了在NX指定图层上创建组件的功能，利用了NX的模型、装配、图层等API，通过交互方式获取用户输入的图层号，遍历图层上的实体，并调用NX的创建组件API将它们创建为一个组件。
>
