### 【1372】highlight all objects on each drawing 在每张绘图上高亮显示所有对象

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_OBJECTS_ON_EACH_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog 是 V18 版本中新增的函数。

仅提供翻译结果，不添加任何额外内容： */  
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
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
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
    static int ask_view_dependent_objects(char *view, tag_t **objects)  
    {  
        tag_t  
            an_object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(uc6457(view, 2, &an_object)) && (an_object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, an_object));  
        return (make_an_array(&object_list, objects));  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            dwg = NULL_TAG,  
            *objects,  
            part = UF_PART_ask_display_part();  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            messg[133],  
            vname[MAX_ENTITY_NAME_SIZE+3];  
        while ((dwg = ask_next_drawing(part, dwg)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_open_drawing(dwg));  
            UF_CALL(UF_OBJ_ask_name(dwg, dname));  
            strcpy(vname, dname);  
            strcat(vname, "@0");  
            n = ask_view_dependent_objects(vname, &objects);  
            set_highlight_object_array(n, objects, TRUE);  
            sprintf(messg, "%d objects on %s", n, dname);  
            uc1601(messg, TRUE);  
            set_highlight_object_array(n, objects, FALSE);  
            if (n > 0) UF_free(objects);  
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

> 这段代码是一个NX Open C++的二次开发示例，主要实现的功能是在每个绘图上高亮显示所有视图相关对象。以下是代码的详细功能介绍：
>
> 1. 包含必要的NX Open C++头文件，如uf.h, uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义了一个函数ask_next_drawing，用于遍历部件中的绘图。
> 4. 定义了一个函数allocate_memory，用于分配内存。
> 5. 定义了一个函数make_an_array，用于将列表转换为数组。
> 6. 定义了一个函数ask_view_dependent_objects，用于查询视图相关的对象列表。
> 7. 定义了一个函数set_highlight_object_array，用于设置对象的高亮显示。
> 8. 定义了一个函数do_it，用于遍历部件中的每个绘图，打开绘图，查询视图相关的对象，并高亮显示这些对象。
> 9. 定义了ufusr函数，这是NX二次开发的主函数，用于初始化NX Open环境，调用do_it函数，然后终止NX Open环境。
> 10. 定义了一个函数ufusr_ask_unload，用于卸载二次开发程序。
>
> 总的来说，这段代码实现了在一个NX部件的每个绘图上高亮显示所有视图相关对象的功能，通过遍历绘图，查询视图相关的对象，并调用API设置高亮显示。
>
