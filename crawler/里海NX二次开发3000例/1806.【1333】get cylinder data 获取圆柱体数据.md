### 【1333】get cylinder data 获取圆柱体数据

#### 代码

```cpp
    /*HEAD GET_CYLINDER_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_vec.h>  
    #include <uf_obj.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的，只需提供译文，无需赘述。 */  
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
    /* qq3123197280 */  
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
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
            cnt,  
            edge_type,  
            n_edges,  
            ii;  
        tag_t  
            *edges,  
            face_tag;  
        double  
            end[3] = { 0,0,0 },  
            length,  
            start[3] = { 0,0,0 };  
        uf_list_p_t  
            edge_list;  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        while ((face_tag = select_a_cylindrical_face("Select a face")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_face_edges(face_tag, &edge_list));  
            if ((n_edges = make_an_array(&edge_list, &edges)) > 0)  
            {  
                cnt = 0;  
                for (ii = 0; ii < n_edges; ii++)  
                {  
                    UF_CALL(UF_MODL_ask_edge_type(edges[ii], &edge_type));  
                    if (edge_type == UF_MODL_CIRCULAR_EDGE)  
                    {  
                        UF_CALL(UF_EVAL_initialize(edges[ii], &evaluator));  
                        UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
                        UF_CALL(UF_EVAL_free(evaluator));  
                        if (cnt == 0)  
                        {  
                            WRITE_F(e_arc.radius);  
                            UF_VEC3_copy(e_arc.center, start);  
                            cnt++;  
                        }  
                        else  
                            UF_VEC3_copy(e_arc.center, end);  
                    }  
                }  
                UF_VEC3_distance(start, end, &length);  
                WRITE3F(start);  
                WRITE3F(end);  
                WRITE_F(length);  
                UF_free(edges);  
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

> 这段代码是一个NX的二次开发示例，主要功能是获取圆柱面数据并输出到日志窗口。
>
> 1. 包含必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于在出现错误时输出错误信息到系统日志和日志窗口。
> 3. 定义了一个选择圆柱面函数select_a_cylindrical_face，用于提示用户选择一个圆柱面。
> 4. 定义了内存分配函数allocate_memory，用于分配内存。
> 5. 定义了列表转换为数组函数make_an_array，用于将UF列表转换为数组。
> 6. 定义了输出浮点数到日志窗口的宏WRITE_F。
> 7. 定义了输出浮点数组到日志窗口的函数WRITE3F。
> 8. 定义了主函数do_it，用于获取选定圆柱面的边，计算边长度并输出到日志窗口。
> 9. 定义了UF启动和终止函数ufusr。
> 10. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码通过交互式选择获取圆柱面，计算相关参数并输出，展示了NX二次开发的基本流程。
>
