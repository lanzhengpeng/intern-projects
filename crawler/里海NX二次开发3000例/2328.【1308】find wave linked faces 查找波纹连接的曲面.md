### 【1308】find wave linked faces 查找波纹连接的曲面

#### 代码

```cpp
    /*HEAD FIND_WAVE_LINKED_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        printf("%d = %s\n", feat, string);  
        UF_free(string);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            face = NULL_TAG,  
            *feats,  
            part = UF_PART_ask_display_part();  
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
                    report_feature_name(feats[ii]);  
                    UF_CALL(UF_DISP_set_highlight(face, TRUE));  
                }  
                UF_free(type);  
            }  
            UF_free(feats);  
        }  
        uc1601("linked faces are highlighted", TRUE);  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是高亮显示零件中的所有关联面特征。代码的详细流程如下：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出错误信息和调用函数。
> 2. 获取下一个面：定义了一个函数ask_next_face，用于遍历零件中的所有面，并返回下一个活动面。
> 3. 分配内存：定义了一个函数allocate_memory，用于分配内存。
> 4. 创建数组：定义了一个函数make_an_array，用于将特征列表转换成特征数组，并释放列表内存。
> 5. 报告特征名称：定义了一个函数report_feature_name，用于打印特征名称。
> 6. 处理函数：定义了do_it函数，用于遍历零件中的所有面，获取每个面的特征列表，检查特征类型是否为"LINKED_FACE"，如果是则高亮显示该面。
> 7. 主函数：定义了ufusr主函数，用于初始化NX，调用do_it函数处理，然后终止NX。
> 8. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载应用程序时返回立即卸载标志。
>
> 总体而言，这段代码通过遍历零件中的所有面，并高亮显示类型为"LINKED_FACE"的特征面，实现了在NX中高亮显示关联面的功能。
>
