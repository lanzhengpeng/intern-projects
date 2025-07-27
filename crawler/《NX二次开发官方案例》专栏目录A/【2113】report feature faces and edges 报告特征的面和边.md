### 【2113】report feature faces and edges 报告特征的面和边

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_UI_feat_sel_type_t  
            add_booleans = UF_UI_FEAT_SEL_TYPE_BROWSEABLE;  
        UF_CALL(UF_UI_select_feature(prompt, &add_booleans, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
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
    static int ask_feat_faces(tag_t feat, tag_p_t *feat_faces)  
    {  
        int  
            ii,  
            jj,  
            n_faces,  
            n_feats;  
        tag_t  
            *faces,  
            *feats;  
        uf_list_p_t  
            face_list,  
            feat_list,  
            partial_list;  
        char  
            *type;  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if (!strcmp(type, "UDF_INS") ||  
            !strcmp(type, "MIRROR_SET") ||  
            !strcmp(type, "INSTNACE_SET") ||  
            !strcmp(type, "INSTANCE_SET") ||  
            !strcmp(type, "LINEAR_PATTERN") ||  
            !strcmp(type, "CIRCULAR_PATTERN") ||  
            !strcmp(type, "FSET"))  
        {  
            UF_CALL(UF_MODL_create_list(&face_list));  
            UF_CALL(UF_MODL_ask_all_members_of_set(feat, &feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                n_faces = ask_feat_faces(feats[ii], &faces);  
            /*  Only include faces whose first feature is the feature in question 里海译:仅包括第一个特征为所讨论特征的面孔。 */  
                for (jj = 0; jj < n_faces; jj++)  
                {  
                    UF_CALL(UF_MODL_ask_face_feats(faces[jj], &feat_list));  
                    if (feat_list->eid == feats[ii])  
                        UF_CALL(UF_MODL_put_list_item(face_list, faces[jj]));  
                    UF_CALL(UF_MODL_delete_list(&feat_list));  
                }  
                if (n_faces > 0) UF_free(faces);  
            }  
            if (n_feats > 0) UF_free(feats);  
        }  
        else if (!strcmp(type, "LINEAR_ISET") ||  
                 !strcmp(type, "CIRCULAR_ISET"))  
        {  
            UF_CALL(UF_MODL_create_list(&face_list));  
            UF_CALL(UF_MODL_ask_instance(feat, &partial_list));  
            n_feats = make_an_array(&partial_list, &feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                n_faces = ask_feat_faces(feats[ii], &faces);  
            /*  Only include faces whose first feature is the feature in question 里海译:只包括那些第一个特征是所讨论特征的面孔。 */  
                for (jj = 0; jj < n_faces; jj++)  
                {  
                    UF_CALL(UF_MODL_ask_face_feats(faces[jj], &feat_list));  
                    if (feat_list->eid == feats[ii])  
                        UF_CALL(UF_MODL_put_list_item(face_list, faces[jj]));  
                    UF_CALL(UF_MODL_delete_list(&feat_list));  
                }  
                if (n_faces > 0) UF_free(faces);  
            }  
            if (n_feats > 0) UF_free(feats);  
        }  
        else UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        UF_free(type);  
        return make_an_array(&face_list, feat_faces);  
    }  
    static int ask_feat_edges(tag_t feat, tag_p_t *feat_edges)  
    {  
        int  
            ii,  
            n_faces;  
        tag_t  
            *faces;  
        uf_list_p_t  
            edge_list,  
            partial_list,  
            tmp_list;  
        UF_CALL(UF_MODL_create_list(&edge_list));  
        n_faces = ask_feat_faces(feat, &faces);  
        for (ii = 0; ii < n_faces; ii++)  
        {  
            UF_CALL(UF_MODL_ask_face_edges(faces[ii], &partial_list));  
            for (tmp_list = partial_list; tmp_list != NULL; tmp_list = tmp_list->next)  
                UF_CALL(UF_MODL_put_list_item(edge_list, tmp_list->eid));  
            UF_CALL(UF_MODL_delete_list(&partial_list));  
        }  
        if (n_faces > 0) UF_free(faces);  
        return make_an_array(&edge_list, feat_edges);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void report_feature_faces_and_edges(tag_t feat)  
    {  
        int  
            n_edges,  
            n_faces;  
        tag_t  
            *edges,  
            *faces;  
        n_faces = ask_feat_faces(feat, &faces);  
        WRITE_D(n_faces);  
        if (n_faces > 0)  
        {  
            set_highlight_object_array(n_faces, faces, TRUE);  
            uc1601("Feature faces", TRUE);  
            set_highlight_object_array(n_faces, faces, FALSE);  
            UF_free(faces);  
        }  
        n_edges = ask_feat_edges(feat, &edges);  
        WRITE_D(n_edges);  
        if (n_edges > 0)  
        {  
            set_highlight_object_array(n_edges, edges, TRUE);  
            uc1601("Feature edges", TRUE);  
            set_highlight_object_array(n_edges, edges, FALSE);  
            UF_free(edges);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
            report_feature_faces_and_edges(feat);  
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

> 根据代码内容，这是一段基于NX Open C++ API的NX二次开发代码，主要用于选择特征并高亮显示其相关的面和边。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了一个选择特征的函数select_a_feature，用于让用户选择一个特征，并返回特征tag。
> 3. 定义了设置高亮显示的函数set_highlight_object_array，用于高亮显示一个对象数组。
> 4. 定义了获取特征面的函数ask_feat_faces，用于递归获取特征相关的所有面。
> 5. 定义了获取特征边的函数ask_feat_edges，用于获取特征相关的所有边。
> 6. 定义了报告特征的面和边的函数report_feature_faces_and_edges，用于高亮显示特征相关的面和边。
> 7. 定义了主函数do_it，用于循环选择特征，并报告其面和边。
> 8. 定义了ufusr函数，这是NX二次开发的入口函数，用于初始化并调用主函数。
> 9. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 整体来说，这段代码通过NX Open C++ API实现了一个选择特征并高亮显示其相关的面和边的基本功能，展示了NX二次开发的基本流程和常用函数的使用方法。
>
