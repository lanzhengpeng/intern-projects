### 【1374】highlight all pmi display instances 高亮所有PMI显示实例

#### 代码

```cpp
    锘/*HEAD HIGHLIGHT_ALL_PMI_DISPLAY_INSTANCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
    #include <uf_layer.h>  
    #include <uf_assem.h>  
    static void ECHO(char* format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char* file, int line, char* call, int irc)  
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
    static int allocate_memory(unsigned int nbytes, void** where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t* object_list, tag_t** objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void**)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_drf_objects(tag_t * *objects)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object))  
            && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type == UF_drafting_entity_type) ||  
                (type == UF_dimension_type) ||  
                (type == UF_note_type) ||  
                (type == UF_tol_feature_instance_type) ||  
                ((type == UF_tabular_note_type) &&  
                (subtype == UF_tabular_note_section_subtype)) ||  
                    (type == UF_smart_model_instance_type) ||  
                (type == UF_pmi_filter_type))  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }  
    static void set_highlight_object_list(uf_list_p_t list, int flag)  
    {  
        uf_list_p_t  
            tmp;  
        for (tmp = list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_DISP_set_highlight(tmp->eid, flag));  
    }  
    static void set_highlight_object_array(int n, tag_t * objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            isOcc = FALSE, isInhPmi = FALSE, isDispInst = FALSE;  
        int  
            ii,  
            numDrf = 0, numPmi = 0;  
        tag_t  
            * objects;  
        uf_list_p_t  
            list;  
        char  
            msg[MAX_LINE_SIZE];  
        numDrf = ask_all_drf_objects(&objects);  
        if (numDrf == 0) return;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < numDrf; ii++)  
        {  
            isOcc = UF_ASSEM_is_occurrence(objects[ii]);  
            //ECHO("Drafting Object %d\n", objects[ii]);  
            //ECHO(" Occurrence? %d\n", isOcc);  
            if (!isOcc)  
            {  
                UF_CALL(UF_DRF_is_inherited_pmi(objects[ii], &isInhPmi));  
                //ECHO(" Inherited PMI? %d\n", isInhPmi);  
                UF_CALL(UF_DRF_is_pmi_display_instance(objects[ii], &isDispInst));  
                //ECHO(" PMI DIsplay Instance? %d\n", isDispInst);  
            }  
            if (isDispInst) UF_CALL(UF_MODL_put_list_item(list, objects[ii]));  
        }  
        UF_free(objects);  
        UF_CALL(UF_MODL_ask_list_count(list, &numPmi));  
        if (numPmi > 0)  
        {  
            set_highlight_object_list(list, TRUE);  
            sprintf(msg, "%d PMI display instance(s) amoung %d drafting objects highlighted", numPmi, numDrf);  
            uc1601(msg, TRUE);  
            set_highlight_object_list(list, FALSE);  
            UF_CALL(UF_MODL_delete_list(&list));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char* param, int* retcode, int paramLen)  
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

> 这段代码是一个NX Open Application的二次开发示例，其主要功能是高亮显示NX装配模型中的PMI显示实例。下面是对代码主要功能的介绍：
>
> 1. 头文件引入：引入了NX Open Application开发所需的头文件，如uf.h, uf_ui.h等。
> 2. 宏定义：定义了UF_CALL宏，用于调用UF函数，并在出错时打印错误信息。
> 3. 错误处理：实现了report_error函数，用于在UF函数调用出错时，打印文件名、行号、函数调用等信息。
> 4. 内存分配：实现了allocate_memory函数，用于为NX对象数组分配内存。
> 5. 对象数组转换：实现了make_an_array函数，用于将NX对象列表转换为对象数组。
> 6. 查询所有DRF对象：实现了ask_all_drf_objects函数，用于查询装配中的所有DRF对象，如线条、尺寸等。
> 7. 高亮设置：实现了set_highlight_object_list和set_highlight_object_array函数，用于设置对象的高亮显示。
> 8. 高亮PMI显示实例：实现了do_it函数，用于遍历所有DRF对象，高亮显示其中的PMI显示实例。
> 9. 入口函数：实现了ufusr函数，作为NX Open Application的入口函数，调用do_it实现主要功能。
> 10. 卸载函数：实现了ufusr_ask_unload函数，用于卸载应用程序。
>
> 综上所述，该代码通过遍历查询装配中的DRF对象，找出其中的PMI显示实例，并通过高亮显示的方式突出显示这些实例，帮助用户直观地识别模型中的PMI信息。
>
