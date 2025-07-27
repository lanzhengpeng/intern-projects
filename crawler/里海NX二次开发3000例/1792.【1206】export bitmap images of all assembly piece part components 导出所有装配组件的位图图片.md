### 【1206】export bitmap images of all assembly piece part components 导出所有装配组件的位图图片

#### 代码

```cpp
    /*HEAD EXPORT_BITMAP_IMAGES_OF_ALL_ASSEMBLY_PIECE_PART_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。

翻译内容：
UF_print_syslog是V18版本新增的函数，仅回答译文，不要废话。 */  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static int ask_all_assembly_piece_parts(tag_t part, tag_t **parts)  
    {  
        tag_t  
            comp = NULL_TAG,  
            comp_part;  
        uf_list_p_t  
            part_list;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            if ((UF_ASSEM_ask_root_part_occ(comp_part)) == NULL_TAG)  
                UF_CALL(UF_MODL_put_list_item(part_list, comp_part));  
        }  
        return make_an_array(&part_list, parts);  
    }  
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        logical  
            tceng;  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(fspec, temp_name, dspec,  
                        dspec, dspec));  
        }  
        else  
        {  
            UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
            if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        }  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *piece_parts;  
        char  
            pspec[MAX_FSPEC_SIZE+1],  
            ispec[MAX_FSPEC_SIZE+1];  
        n = ask_all_assembly_piece_parts(part, &piece_parts);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_PART_set_display_part(piece_parts[ii]));  
            UF_CALL(UF_PART_ask_part_name(piece_parts[ii], pspec));  
            build_similar_temp_filespec(pspec, 0, ispec);  
            strcat(ispec, ".bmp");  
            UF_CALL(UF_DISP_create_image(ispec, UF_DISP_BMP, UF_DISP_ORIGINAL));  
        }  
        if (n > 0)  
        {  
            UF_CALL(UF_PART_set_display_part(part));  
            UF_free(piece_parts);  
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

> 这段代码是用于NX的二次开发，其主要功能是导出装配体中的所有零部件组件的位图图像。具体来说：
>
> 1. 初始化和错误处理：代码首先包含必要的NX头文件，并定义了一个错误处理函数report_error，用于在函数调用出错时打印错误信息。
> 2. 内存分配和数组构建：allocate_memory用于申请内存，make_an_array用于将对象列表转换为对象数组。
> 3. 遍历零部件：ask_next_component用于遍历装配体中的下一个组件，ask_all_assembly_piece_parts用于获取所有零部件组件。
> 4. 临时文件构建：build_similar_temp_filespec用于构建临时文件路径。
> 5. 图像导出：do_it函数用于遍历零部件，设置当前显示的零部件，构建临时文件名，并调用UF_DISP_create_image导出位图图像。
> 6. UFusr主函数：该函数初始化NX，调用do_it执行图像导出，最后终止NX。
> 7. 卸载函数：ufusr_ask_unload用于卸载NX时返回立即卸载标志。
>
> 总体而言，这段代码实现了遍历装配体中的零部件，并导出每个零部件的位图图像，展现了NX二次开发中常用的接口调用和文件操作技巧。
>
