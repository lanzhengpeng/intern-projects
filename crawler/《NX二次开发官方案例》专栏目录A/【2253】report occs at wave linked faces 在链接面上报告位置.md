### 【2253】report occs at wave linked faces 在链接面上报告位置

#### 代码

```cpp
    /*HEAD REPORT_OCCS_AT_WAVE_LINKED_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <stdarg.h>  
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
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            status,  
            subtype,  
            type;  
        while ((! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face)))  
            && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            status = UF_OBJ_ask_status(face);  
            if ((subtype == UF_solid_face_subtype) && (status == UF_OBJ_ALIVE))  
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
    static tag_t ask_next_wave_linked_face(tag_t part, tag_t face, tag_t *link_feat)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats;  
        char  
            *type;  
        uf_list_p_t  
            feat_list;  
        while ((face = ask_next_face(part, face)) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(face)) continue;  
            UF_CALL(UF_MODL_ask_face_feats(face, &feat_list));  
            n_feats = make_an_array(&feat_list, &feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feats[ii], &type));  
                if (!strcmp(type, "LINKED_FACE"))  
                {  
                    UF_free(type);  
                    *link_feat = feats[ii];  
                    UF_free(feats);  
                    return face;  
                }  
                UF_free(type);  
            }  
            UF_free(feats);  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            ok;  
        tag_t  
            from_occ,  
            feat,  
            link_face = NULL_TAG,  
            occ_face = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            source_face = NULL_TAG,  
            to_occ,  
            xform;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((link_face = ask_next_wave_linked_face(part, link_face, &feat))  
            != NULL_TAG)  
        {  
            ECHO("Linked Face: %d\n", link_face);  
            UF_CALL(UF_WAVE_ask_link_xform(feat, &xform));  
            if ((xform != NULL_TAG) &&  
                !UF_CALL(UF_SO_is_assy_ctxt_xform(xform, &ok)) && ok)  
            {  
                to_occ = UF_ASSEM_ask_root_part_occ(part);  
                UF_CALL(UF_SO_ask_assy_ctxt_part_occ(xform, to_occ, &from_occ));  
            /*  PR 4119374 - must insure component is fully loaded 里海译:必须确保组件完全加载。 */  
                UF_CALL(UF_ASSEM_ask_component_data(from_occ, part_name,  
                    refset_name, instance_name, origin, csys_matrix, transform));  
                if ((UF_PART_is_loaded(part_name)) != 1)  
                {  
                    UF_CALL(UF_ASSEM_set_work_occurrence(from_occ));  
                    UF_CALL(UF_ASSEM_set_work_occurrence(to_occ));  
                }  
                UF_CALL(UF_WAVE_ask_link_source(feat, TRUE, &source_face));  
                ECHO("Source Face: %d\n", source_face);  
                occ_face = UF_ASSEM_find_occurrence(from_occ, source_face);  
                ECHO("Occurrence of Face: %d\n", occ_face);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发程序，主要用于遍历零件中的所有面，并找出与波特征链接的面。其主要功能包括：
>
> 1. 定义错误报告函数ECHO：用于输出错误信息。
> 2. 定义UF_CALL宏：用于调用NX Open API函数，并在发生错误时输出错误信息。
> 3. 定义ask_next_face函数：用于遍历零件中的所有面。
> 4. 定义allocate_memory和make_an_array函数：用于分配内存并从链表获取对象数组。
> 5. 定义ask_next_wave_linked_face函数：用于遍历零件中的所有面，并找出与波特征链接的面。
> 6. 定义do_it函数：用于遍历所有链接面，获取源面，并找到源面在装配中的实例。
> 7. ufusr函数：程序的入口函数，初始化NX Open，调用do_it执行主体功能，然后终止NX Open。
> 8. ufusr_ask_unload函数：用于卸载动态库。
>
> 总体来说，这段代码实现了从NX装配中找出与波特征链接的面，并找到其在源零件中的实例的功能。
>
