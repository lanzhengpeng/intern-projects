### 【0683】create duplicate copies of all drawings in current part 创建当前部件中所有图纸的副本

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf(msg, UF_UI_MAX_STRING_LEN, format, args);  
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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_model_curves(tag_t part, tag_t **curves)  
    {  
        int  
            flag,  
            ii, jj,  
            n,  
            type[5] = { UF_point_type,  
                        UF_line_type,  
                        UF_circle_type,  
                        UF_conic_type,  
                        UF_spline_type };  
        tag_t  
            *objects;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < 5; ii++)  
        {  
            n = ask_all_of_type(part, type[ii], &objects);  
            for (jj = 0; jj < n; jj++)  
            {  
                UF_CALL(uc6409(objects[jj], &flag, view_name));  
                if (!flag)  
                    UF_CALL(UF_MODL_put_list_item(object_list, objects[jj]));  
            }  
            if (n > 0) UF_free(objects);  
        }  
        return make_an_array(&object_list, curves);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings,  
            n_curves,  
            n_objects;  
        tag_t  
            which_part = NULL_TAG,  
            *drawings,  
            *curves,  
            *objects,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            object_list;  
        char  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_maintain_all_params, UF_PART_copy_exp_deeply };  
        UF_CALL(UF_PART_ask_part_name(part, part_fspec));  
        UF_CALL(UF_MODL_create_list(&object_list));  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        if (n_drawings == 0) return;  
        for (ii = 0; ii < n_drawings; ii++)  
            UF_CALL(UF_MODL_put_list_item(object_list, drawings[ii]));  
        UF_free(drawings);  
    //  Just in case any dimensions are associated to model mode curve objects in  
    //  this part.  See PR 6723668  
        n_curves = ask_all_model_curves(part, &curves);  
        for (ii = 0; ii < n_curves; ii++)  
            UF_CALL(UF_MODL_put_list_item(object_list, curves[ii]));  
        if (n_curves > 0) UF_free(curves);  
        n_objects = make_an_array(&object_list, &objects);  
        if (n_objects > 0)  
        {  
            ECHO("Exporting %d objects to %s\n", n_objects, part_fspec);  
            UF_CALL(UF_PART_export_with_options(part_fspec, n_objects,  
                objects, &options));  
            UF_free(objects);  
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

> 这段代码是一个用于NX的二次开发程序，主要功能是从当前打开的NX部件中导出指定的对象。以下是代码的主要逻辑：
>
> 1. 头文件包含：引入了NX Open API的头文件，包括图形绘制、零件、装配、特征识别、建模、对象和视图等。
> 2. 宏定义：定义了UF_CALL宏，用于调用NX API函数，并报告错误信息。
> 3. 错误处理函数：定义了report_error函数，用于在API调用失败时报告错误。
> 4. 内存分配函数：定义了allocate_memory函数，用于分配NX API所需的内存。
> 5. 创建对象数组函数：定义了make_an_array函数，用于将NX对象列表转换为对象数组。
> 6. 获取指定类型对象函数：定义了ask_all_of_type函数，用于获取部件中指定类型的所有对象。
> 7. 获取模型曲线函数：定义了ask_all_model_curves函数，用于获取部件中的所有模型曲线。
> 8. 导出函数：定义了do_it函数，用于导出当前打开的部件中的绘图、模型曲线等对象。
> 9. 主函数：定义了ufusr函数，作为程序的入口点，调用do_it函数执行导出操作。
> 10. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载插件时返回立即卸载标志。
>
> 综合来看，这段代码实现了从NX部件中获取特定对象并导出的功能，适用于NX的二次开发。
>
