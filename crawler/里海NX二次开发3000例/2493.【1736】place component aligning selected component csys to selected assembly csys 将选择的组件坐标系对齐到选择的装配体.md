### 【1736】place component aligning selected component csys to selected assembly csys 将选择的组件坐标系对齐到选择的装配体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_undo.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_trns.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
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
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static logical open_part(char *fspec, tag_t *part)  
    {  
        UF_PART_load_status_t  
            status;  
        if (!UF_PART_is_loaded(fspec))  
        {  
            UF_CALL(UF_PART_open(fspec, part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            return (!status.failed);  
        }  
        else  
        {  
            *part = UF_PART_ask_part_tag(fspec);  
            if (!UF_CALL(UF_PART_set_display_part(*part)))  
                return TRUE;  
            else  
                return FALSE;  
        }  
    }  
    static tag_t specify_csys(char *title)  
    {  
        int  
            method = 4, // inferred !  
            irc;  
        tag_t  
            csys = NULL_TAG;  
        double  
            matrix[9],  
            origin[3];  
        irc = UF_UI_specify_csys(title, &method, matrix, origin, &csys);  
        return csys;  
    }  
    static tag_t create_duplicate_alive_normal_csys(tag_t csys)  
    {  
        tag_t  
            matrix,  
            new_csys;  
        double  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_create_csys(origin, matrix, &new_csys));  
        return new_csys;  
    }  
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static tag_t add_component(char *fspec)  
    {  
        tag_t  
            instance = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            origin[3],  
            matrix[9];  
        UF_PART_load_status_t  
            error_status;  
        ask_wcs_info(origin, matrix);  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, fspec, NULL, NULL,  
            origin, matrix, 0, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        return (UF_ASSEM_ask_part_occ_of_inst(UF_ASSEM_ask_root_part_occ(part),  
            instance));  
    }  
    static tag_t create_component_csys(tag_t component)  
    {  
        tag_t  
            csys,  
            matrix;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4],  
            zero[3] = { 0,0,0 };  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_CALL(UF_CSYS_create_matrix(csys_matrix, &matrix));  
        UF_CALL(UF_CSYS_create_temp_csys(zero, matrix, &csys));  
        return csys;  
    }  
    static void transform_csys2csys(tag_t ref_csys, tag_t dest_csys, double mx[16])  
    {  
        double  
            csys[9],  
            orig[3],  
            tx1[16],  
            tx2[16],  
            tx3[16],  
            tx4[16],  
            v[3];  
        int  
            ii;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));  
        tx1[0] = csys[0];  
        tx1[1] = csys[1];  
        tx1[2] = csys[2];  
        tx1[3] = 0;  
        tx1[4] = csys[3];  
        tx1[5] = csys[4];  
        tx1[6] = csys[5];  
        tx1[7] = 0;  
        tx1[8] = csys[6];  
        tx1[9] = csys[7];  
        tx1[10] = csys[8];  
        tx1[11] = 0;  
        /* set up to translate from reference csys back to absolute 里海译:根据参考坐标系将绝对坐标系翻译回去 */  
        for (ii=0; ii<3; ii++) v[ii] = -orig[ii];  
        FTN(uf5943)(v, tx2);  
        /* combine this with the rotation matrix from the reference csys 里海译:与参考坐标系中的旋转矩阵结合 */  
        FTN(uf5942)(tx2, tx1, tx3);  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));  
        /* Invert the rotation from the destination matrix 里海译:翻转目标矩阵中的旋转 */  
        tx2[0] = csys[0];  
        tx2[1] = csys[3];  
        tx2[2] = csys[6];  
        tx2[3] = 0;  
        tx2[4] = csys[1];  
        tx2[5] = csys[4];  
        tx2[6] = csys[7];  
        tx2[7] = 0;  
        tx2[8] = csys[2];  
        tx2[9] = csys[5];  
        tx2[10] = csys[8];  
        tx2[11] = 0;  
        /* set up to translate from abs to the destination csys 里海译:设置将ABS转换到目标CSYS的转换。 */  
        FTN(uf5943)(orig, tx1);  
        /* combine this with the inverted rotation csys above 里海译:结合上面的反转旋转坐标系统 */  
        FTN(uf5942)(tx2, tx1, tx4);  
        /* and the one from the reference csys 里海译:根据参考坐标系 */  
        FTN(uf5942)(tx3, tx4, mx);  
    }  
    static void transform_csys_to_csys(tag_t from_csys, tag_t to_csys, int n,  
        tag_t *objects)  
    {  
        int  
            move = 1,  
            work = -1,  
            off = 2,  
            resp;  
        double  
            mx[16];  
        transform_csys2csys(from_csys, to_csys, mx);  
        FTN(uf5947)(mx, objects, &n, &move, &work, &off, NULL, NULL, &resp);  
    }  
    static void ask_csys_info(tag_t csys, double origin[3], double axes[9])  
    {  
        tag_t  
            matrix;  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, axes));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            assy_part = UF_PART_ask_display_part(),  
            assy_csys,  
            comp,  
            comp_csys,  
            comp_csys_occ,  
            comp_part,  
            from_csys,  
            temp_csys,  
            to_csys;  
        double  
            new_origin[3],  
            new_csys_matrix[9];  // need 9 for ask_csys_info  
        char  
            comp_spec[MAX_FSPEC_SIZE+1] = { "" };  
        while (prompt_for_existing_part_name("Component", comp_spec))  
        {  
            if ((!open_part(comp_spec, &comp_part)) ||  
                (from_csys = specify_csys("From CSYS")) == NULL_TAG) continue;  
            comp_csys = create_duplicate_alive_normal_csys(from_csys);  
            UF_CALL(UF_PART_set_display_part(assy_part));  
            if ((to_csys = specify_csys("To CSYS")) == NULL_TAG) continue;  
            assy_csys = create_duplicate_alive_normal_csys(to_csys);  
            set_undo_mark("Place component");  
            comp = add_component(comp_spec);  
            comp_csys_occ = UF_ASSEM_find_occurrence(comp, comp_csys);  
            temp_csys = create_component_csys(comp);  
            transform_csys_to_csys(comp_csys_occ, assy_csys, 1, &temp_csys);  
            ask_csys_info(temp_csys, new_origin, new_csys_matrix);  
            UF_CALL(UF_ASSEM_reposition_instance(  
                UF_ASSEM_ask_inst_of_part_occ(comp), new_origin,  
                new_csys_matrix));  
            UF_CALL(UF_OBJ_delete_object(comp_csys));  
            UF_CALL(UF_OBJ_delete_object(assy_csys));  
            UF_CALL(UF_DISP_regenerate_display());  
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

> 这段NX二次开发代码主要用于在NX装配中添加组件，并设置组件的坐标系。下面是代码的主要功能和流程：
>
> 1. 功能介绍：提示用户输入组件文件名，并打开组件。指定组件的原始坐标系和目标坐标系。将组件添加到装配中，并设置组件的位置和方向。生成临时坐标系，将组件坐标系变换到装配坐标系。获取变换后的坐标系信息，并重新设置组件的位置和方向。删除临时坐标系，刷新显示。
> 2. 提示用户输入组件文件名，并打开组件。
> 3. 指定组件的原始坐标系和目标坐标系。
> 4. 将组件添加到装配中，并设置组件的位置和方向。
> 5. 生成临时坐标系，将组件坐标系变换到装配坐标系。
> 6. 获取变换后的坐标系信息，并重新设置组件的位置和方向。
> 7. 删除临时坐标系，刷新显示。
> 8. 主要流程：提示用户输入组件文件名，并打开组件。指定组件的原始坐标系。指定组件的目标坐标系。将组件添加到装配中。生成临时坐标系，将组件坐标系变换到装配坐标系。获取变换后的坐标系信息。重新设置组件的位置和方向。删除临时坐标系，刷新显示。
> 9. 提示用户输入组件文件名，并打开组件。
> 10. 指定组件的原始坐标系。
> 11. 指定组件的目标坐标系。
> 12. 将组件添加到装配中。
> 13. 生成临时坐标系，将组件坐标系变换到装配坐标系。
> 14. 获取变换后的坐标系信息。
> 15. 重新设置组件的位置和方向。
> 16. 删除临时坐标系，刷新显示。
> 17. 关键函数：prompt_for_existing_part_name：提示用户输入组件文件名。open_part：打开组件。specify_csys：指定坐标系。add_component：将组件添加到装配中。create_component_csys：创建组件的临时坐标系。transform_csys_to_csys：变换坐标系。ask_csys_info：获取坐标系信息。UF_ASSEM_reposition_instance：重新设置组件位置和方向。UF_DISP_regenerate_display：刷新显示。
> 18. prompt_for_existing_part_name：提示用户输入组件文件名。
> 19. open_part：打开组件。
> 20. specify_csys：指定坐标系。
> 21. add_component：将组件添加到装配中。
> 22. create_component_csys：创建组件的临时坐标系。
> 23. transform_csys_to_csys：变换坐标系。
> 24. ask_csys_info：获取坐标系信息。
> 25. UF_ASSEM_reposition_instance：重新设置组件位置和方向。
> 26. UF_DISP_regenerate_display：刷新显示。
> 27. 注意事项：使用UF_UNDO_set_mark设置撤销标记。使用UF_UI_set_prompt设置提示信息。使用UF_get_fail_message获取错误信息。使用UF_PART_free_load_status释放加载状态。使用UF_DISP_regenerate_display刷新显示。
> 28. 使用UF_UNDO_set_mark设置撤销标记。
> 29. 使用UF_UI_set_prompt设置提示信息。
> 30. 使用UF_get_fail_message获取错误信息。
> 31. 使用UF_PART_free_load_status释放加载状态。
> 32. 使用UF_DISP_regenerate_display刷新显示。
>
> 以上是对该NX二次开发代码的主要介绍。代码实现了在装配中添加组件并设置坐标系的功能。
>
