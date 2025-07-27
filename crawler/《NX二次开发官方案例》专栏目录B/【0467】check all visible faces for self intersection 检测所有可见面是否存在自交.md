### 【0467】check all visible faces for self intersection 检测所有可见面是否存在自交

#### 代码

```cpp
    /*HEAD CHECK_ALL_VISIBLE_FACES_FOR_SELF_INTERSECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，可以得知 UF_print_syslog 是 V18 新增的函数。因此，翻译为：

注意：UF_print_syslog 是 V18 新增的函数，只回答翻译，不要赘述。 */  
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
    static tag_t ask_next_visible_face(tag_t face)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &face)) && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_face_subtype))  
                return face;  
        }  
        return NULL_TAG;  
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
    static int ask_all_visible_faces(tag_t **faces)  
    {  
        tag_t  
            face = NULL_TAG;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_create_list(&face_list));  
        while ((face = ask_next_visible_face(face)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(face_list, face));  
        return (make_an_array(&face_list, faces));  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    static void do_it(void)  
    {  
        int  
            flag,  
            ii, jj,  
            n_faces;  
        tag_t  
            *faces;  
        double  
            point[3];  
        char  
            err[133];  
        if ((n_faces = ask_all_visible_faces(&faces)) > 0)  
        {  
            for (ii = 0; ii < n_faces; ii++)  
            {  
                UF_CALL(UF_MODL_ask_face_self_intersect(faces[ii], &flag, point));  
                if (flag)  
                {  
                    UF_get_fail_message(flag, err);  
                    UF_CALL(UF_DISP_set_highlight(faces[ii], TRUE));  
                /*  Work around PR 5101370 译:根据文档内容，这是关于围绕PR 5101370的翻译工作。具体来说，文档提到了一个编号为5101370的公共关系案例，并要求翻译相关内容。文档强调只提供翻译，不要添加任何无关的评论或解释。由于文档未提供具体要翻译的内容，因此无法提供完整的翻译。如果提供具体的公共关系案例内容，我将能进行相应的翻译。 */  
                    for (jj = 0; jj < 3; jj++) point[jj] *= 0.0254;  
                /*  End work around PR 5101370 译:翻译 End work around PR 5101370，只回答译文，不要废话。 */  
                    display_temporary_point(point);  
                    uc1601(err, TRUE);  
                    UF_CALL(UF_DISP_set_highlight(faces[ii], FALSE));  
                }  
            }  
            UF_free(faces);  
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

> 这段代码是NX二次开发的UF程序，主要用于检测和显示模型中所有可见面的自交情况。
>
> 主要功能包括：
>
> 1. 遍历所有可见面：使用UF_VIEW_cycle_objects和UF_OBJ_ask_type_and_subtype函数遍历当前视图中的所有可见对象，并将所有可见的面存入一个列表。
> 2. 检测自交情况：使用UF_MODL_ask_face_self_intersect函数检测每个面是否存在自交，并将自交点坐标存入数组中。
> 3. 显示自交信息：如果面存在自交，使用UF_DISP_set_highlight函数高亮显示该面，并使用UF_DISP_display_temporary_point函数显示自交点的位置，同时打印出错误信息。
> 4. 释放内存：在遍历完所有面后，释放申请的内存。
> 5. 错误处理：使用UF_CALL宏进行错误处理，如果函数调用失败，会打印错误信息。
> 6. 初始化和终止：在ufusr函数中初始化和终止NX Open API。
> 7. 卸载处理：在ufusr_ask_unload函数中设置立即卸载。
>
> 总体来说，这段代码实现了检测模型中所有可见面的自交情况，并以图形化的方式显示出来，有助于发现和修复模型中的错误。
>
