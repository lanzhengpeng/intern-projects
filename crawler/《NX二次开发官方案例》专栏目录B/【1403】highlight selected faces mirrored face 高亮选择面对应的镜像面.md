### 【1403】highlight selected faces mirrored face 高亮选择面对应的镜像面

#### 代码

```cpp
    /*HEAD HIGHLIGHT_SELECTED_FACES_MIRRORED_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本新增的函数，用于打印系统日志。建议翻译为：UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
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
    static int ask_face_feats(tag_t face, tag_t **feats)  
    {  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_face_feats(face, &feat_list));  
        return make_an_array(&feat_list, feats);  
    }  
    static void highlight_mirrored_face(tag_t face)  
    {  
        int  
            ii,  
            jj,  
            n_feats,  
            n_items;  
        char  
            *feat_type;  
        tag_t  
            *feats,  
            mirror,  
            *results,  
            *sources;  
        n_feats = ask_face_feats(face, &feats);  
        for (ii = 0; ii < n_feats; ii++)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feats[ii], &feat_type));  
            if ((strstr(feat_type, "MIRROR")) &&  
                ! UF_CALL(UF_WAVE_map_link_geom_to_source(feats[ii], face, TRUE,  
                    &mirror)))  
            {  
            /*  Begin work around to PR 4244233 里海译:开始围绕PR 4244233展开工作 */  
                if (mirror == NULL_TAG)  
                {  
                    UF_CALL(UF_WAVE_ask_linked_feature_map(feats[ii], TRUE,  
                        &n_items, &sources, &results));  
                    for (jj = 0; jj < n_items; jj++)  
                    {  
                        if (results[jj] == face)  
                        {  
                            mirror = sources[jj];  
                            break;  
                        }  
                    }  
                    if (n_items > 0)  
                    {  
                        UF_free(sources);  
                        UF_free(results);  
                    }  
                }  
            /*  End work around to PR 4244233 里海译:End work around to PR 4244233，请翻译为：结束针对PR 4244233的临时修复。 */  
                UF_DISP_set_highlight(face, TRUE);  
                UF_DISP_set_highlight(mirror, TRUE);  
                uc1601("Mirrored face highlighted", TRUE);  
                UF_DISP_set_highlight(face, FALSE);  
                UF_DISP_set_highlight(mirror, FALSE);  
            }  
            UF_free(feat_type);  
        }  
        if (n_feats > 0) UF_free(feats);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        while ((face = select_a_face("Select face")) != NULL_TAG)  
            highlight_mirrored_face(face);  
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

> 这段代码是基于NX的二次开发代码，主要功能是高亮显示所选面的镜像面。下面是对代码的简要介绍：
>
> 1. 错误处理函数：report_error用于记录NX API调用失败的错误信息，并打印到系统日志和控制台窗口。
> 2. 选择面功能：mask_for_faces和select_a_face函数用于在NX中选择一个面。
> 3. 内存分配和数组处理：allocate_memory和make_an_array用于分配内存和处理NX对象列表。
> 4. 查询面的特征：ask_face_feats函数用于查询一个面的特征列表。
> 5. 高亮显示镜像面：highlight_mirrored_face函数首先查询面的特征，然后找到镜像特征，并高亮显示该特征对应的镜像面。
> 6. 主函数：do_it函数循环调用select_a_face和highlight_mirrored_face，直到用户取消选择。
> 7. NX初始化和终止：ufusr函数用于初始化NX环境，并调用do_it，最后终止NX环境。
> 8. 卸载函数：ufusr_ask_unload表示可以立即卸载该二次开发程序。
>
> 总体来说，这段代码实现了在NX中高亮显示选定面的镜像面的功能，具有一定的实用价值。
>
