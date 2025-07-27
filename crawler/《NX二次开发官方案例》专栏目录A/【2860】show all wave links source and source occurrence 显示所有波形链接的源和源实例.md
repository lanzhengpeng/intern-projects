### 【2860】show all wave links source and source occurrence 显示所有波形链接的源和源实例

#### 代码

```cpp
    /*HEAD SHOW_ALL_WAVE_LINKS_SOURCE_AND_SOURCE_OCCURRENCE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能。 */  
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
    static tag_t ask_next_wave_linked_feature(tag_t part, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (strstr(this_type, "LINK"))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_fully_loaded(tag_t item)  
    {  
        tag_t  
            part;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            error_status;  
        if (UF_ASSEM_is_occurrence(item))  
            item = UF_ASSEM_ask_prototype_of_occ(item);  
        UF_CALL(UF_OBJ_ask_owning_part(item, &part));  
        UF_PART_ask_part_name(part, partname);  
        if ((UF_PART_is_loaded(partname)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            ok;  
        int  
            ii,  
            n_occs;  
        tag_t  
            disp_part = UF_PART_ask_display_part(),  
            feat = NULL_TAG,  
            link_geom,  
            *occs,  
            source_obj_proto = NULL_TAG,  
            source_obj_occ = NULL_TAG,  
            source_part_occ,  
            to_occ,  
            part = UF_ASSEM_ask_work_part(),  
            xform_part,  
            xform;  
        while ((feat = ask_next_wave_linked_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_WAVE_ask_link_xform(feat, &xform));  
            UF_CALL(UF_WAVE_ask_linked_feature_geom(feat, &link_geom));  
            UF_DISP_set_highlight(link_geom, TRUE);  
            uc1601("link_geom", TRUE);  
            UF_DISP_set_highlight(link_geom, FALSE);  
            if ((xform != NULL_TAG) &&  
                !UF_CALL(UF_SO_is_assy_ctxt_xform(xform, &ok)) && ok)  
            {  
                UF_CALL(UF_OBJ_ask_owning_part(xform, &xform_part));  
                n_occs = UF_ASSEM_ask_occs_of_part(disp_part, xform_part, &occs);  
                if (n_occs > 0)  
                {  
                    for (ii = 0; ii < n_occs; ii++)  
                    {  
                        to_occ = occs[ii];  
                        UF_CALL(UF_SO_ask_assy_ctxt_part_occ(xform, to_occ,  
                            &source_part_occ));  
                        if (source_part_occ != NULL_TAG) break;  
                    }  
                    UF_free(occs);  
                }  
                else  
                {  
                    to_occ = UF_ASSEM_ask_root_part_occ(part);  
                    UF_CALL(UF_SO_ask_assy_ctxt_part_occ(xform, to_occ,  
                        &source_part_occ));  
                }  
                UF_DISP_set_highlight(source_part_occ, TRUE);  
                uc1601("source_part_occ", TRUE);  
                UF_DISP_set_highlight(source_part_occ, FALSE);  
                ensure_fully_loaded(source_part_occ);  
                UF_CALL(UF_WAVE_ask_link_source(feat, TRUE, &source_obj_proto));  
                UF_DISP_set_highlight(source_obj_proto, TRUE);  
                uc1601("source_obj_proto", TRUE);  
                UF_DISP_set_highlight(source_obj_proto, FALSE);  
                source_obj_occ = UF_ASSEM_find_occurrence(source_part_occ,  
                    source_obj_proto);  
                UF_DISP_set_highlight(source_obj_occ, TRUE);  
                uc1601("source_obj_occ", TRUE);  
                UF_DISP_set_highlight(source_obj_occ, FALSE);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是遍历当前工作部件中的所有波浪链接特征，并高亮显示特征、源对象原型和源对象实例。以下是代码的主要内容和流程：
>
> 1. 头文件和宏定义：引入了NX API相关的头文件，并定义了UF_CALL宏，用于封装UF函数调用并打印错误信息。
> 2. 错误报告函数：定义了report_error函数，用于在UF函数调用失败时打印错误信息。
> 3. 获取下一个波浪链接特征：定义了ask_next_wave_linked_feature函数，用于遍历部件中的特征，并返回下一个波浪链接特征。
> 4. 报告加载状态：定义了report_load_status函数，用于在打开部件时报告加载状态。
> 5. 确保完全加载：定义了ensure_fully_loaded函数，用于确保指定的对象完全加载。
> 6. 主要功能函数：定义了do_it函数，用于遍历波浪链接特征，获取链接变换、链接几何体、源对象原型和源对象实例，并使用高亮显示。
> 7. UFusr入口函数：定义了ufusr函数，用于初始化NX环境，调用do_it函数，并终止NX环境。
> 8. 卸载请求函数：定义了ufusr_ask_unload函数，用于在插件卸载时立即卸载。
>
> 总的来说，这段代码实现了遍历波浪链接特征，并高亮显示特征、源对象原型和源对象实例的功能，为NX的二次开发提供了有用的参考。
>
