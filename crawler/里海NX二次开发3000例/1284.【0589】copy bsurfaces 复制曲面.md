### 【0589】copy bsurfaces 复制曲面

#### 代码

```cpp
    /*HEAD COPY_BSURFACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能。 */  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    /* 里海 */  
    static int mask_for_bsurfs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PARAMETRIC_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_bsurf(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bsurfs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void open_copy_part(tag_t part, tag_t *copy_part)  
    {  
        int  
            units;  
        char  
            copy_fspec[MAX_FSPEC_SIZE+1];  
        build_copy_partname(copy_fspec);  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (!UF_PART_new(copy_fspec, units, copy_part))  
        {  
            UF_CALL(UF_PART_set_display_part(part));  
            return;  
        }  
        *copy_part = UF_PART_ask_part_tag(copy_fspec);  
        return;  
    }  
    static void do_it(void)  
    {  
        int  
            knot_fixup,  
            pole_fixup;  
        tag_t  
            bsurf,  
            cp_part,  
            orig_part,  
            new_bsurf,  
            part = UF_PART_ask_display_part();  
        UF_MODL_bsurface_t  
            bsurf_data;  
        open_copy_part(part, &cp_part);  
        while ((bsurf = select_a_bsurf("Select bsurf face")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_bsurf(bsurf, &bsurf_data));  
            UF_CALL(UF_ASSEM_set_work_part_quietly(cp_part, &orig_part));  
            UF_CALL(UF_MODL_create_bsurf(  
                bsurf_data.num_poles_u, bsurf_data.num_poles_v,  
                bsurf_data.order_u, bsurf_data.order_v,  
                bsurf_data.knots_u, bsurf_data.knots_v,  
                (double *)bsurf_data.poles,  
                &new_bsurf, &knot_fixup, &pole_fixup));  
            UF_CALL(UF_ASSEM_set_work_part_quietly(orig_part, &cp_part));  
            UF_CALL(UF_MODL_free_bsurf_data(&bsurf_data));  
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

> 这段NX二次开发代码的主要功能是实现选择一个B曲面，然后创建一个新的零件，并在该新零件中复制这个B曲面。以下是代码的详细功能介绍：
>
> 1. 错误报告函数：使用宏UF_CALL来检查NX API函数的返回码，如果有错误，则调用report_error函数来报告错误。
> 2. 构建新零件名称：函数build_copy_partname用于构建一个新零件的文件名。
> 3. 选择B曲面：函数select_a_burf用于让用户选择一个B曲面，并返回其tag。
> 4. 打开新零件：函数open_copy_part用于创建一个新的零件，并返回其tag。
> 5. 复制B曲面：主函数do_it首先打开一个新零件，然后循环让用户选择B曲面，每次选择后，在当前工作零件中复制这个B曲面。
> 6. NX初始化和终止：函数ufusr进行NX初始化，调用do_it函数，然后终止NX。
> 7. 卸载函数：函数ufusr_ask_unload用于在二次开发应用程序卸载时调用。
>
> 通过这些功能，这段代码实现了在NX中复制B曲面的功能。
>
