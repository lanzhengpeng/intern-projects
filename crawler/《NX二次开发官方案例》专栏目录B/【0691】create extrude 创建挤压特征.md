### 【0691】create extrude 创建挤压特征

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_vec.h>  
    #include <uf_sc.h>  
    #include <uf_so.h>  
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
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        objs = *curves;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void ask_zc_dir(double zc_dir[3])  
    {  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        double  
            origin[3],  
            wcs[9];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, wcs));  
        UF_VEC3_copy(&wcs[6], zc_dir);  
    }  
    static tag_t create_dumb_curve_section(int n_curves, tag_t *curves)  
    {  
        int  
            resp;  
        tag_t  
            section;  
        UF_SC_section_data_t  
            sec_rule;  
        UF_MODL_initialize_section_data(&sec_rule);  
        sec_rule.n_rules = 1;  
        sec_rule.rule_types = (int *)UF_allocate_memory(sizeof(int), &resp);  
        sec_rule.rules =  
            (UF_SC_input_data_t *)UF_allocate_memory(sizeof(UF_SC_input_data_t), &resp);  
        UF_MODL_init_sc_input_data(UF_SC_CURVE_DUMB_CURVES, &sec_rule.rules[0]);  
        sec_rule.rule_types[0] = UF_SC_CURVE_DUMB_CURVES;  
        sec_rule.rules[0].curve_dumb_input.num_curves = n_curves;  
        sec_rule.rules[0].curve_dumb_input.curves = curves;  
        UF_CALL(UF_MODL_create_section(curves[0], 1, &sec_rule, NULL_TAG, NULL,  
            NULL, FALSE, &section));  
    /*  NOTE:  This will free the input curves array automatically 译:这将会自动释放输入曲线数组。 */  
        UF_CALL(UF_MODL_free_section_data(&sec_rule));  
        return section;  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves,  
            n_feats;  
        tag_t  
            *curves,  
            *feats;  
        double  
            dir[3];  
        UF_MODL_mswp_extrude_t  
            extrude;  
        if ((n_curves = select_curves("Select section curves", &curves)) > 0)  
        {  
            UF_CALL(UF_MODL_mswp_init_extrude_data(&extrude));  
            ask_zc_dir(dir);  
            UF_CALL(UF_SO_create_dirr_doubles(curves[0],  
                UF_SO_update_within_modeling, dir, &extrude.direction));  
        /*  NOTE:  This UF_free's the curves array 译:根据文档内容，翻译为：

注意：这个函数释放了曲线数组。 */  
            extrude.section = create_dumb_curve_section(n_curves, curves);  
            if (!UF_CALL(UF_MODL_mswp_create_extrude(&extrude, &n_feats, &feats))  
                && (n_feats > 0)) UF_free(feats);  
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

> 根据代码内容，这是用于NX CAD系统的二次开发代码，主要功能如下：
>
> 1. 错误处理：代码定义了错误处理函数ECHO和report_error，用于输出错误信息，便于调试。
> 2. 选择曲线：函数select_curves实现选择曲线功能，提示用户选择曲线，并返回选择的曲线数组。
> 3. 确定方向：函数ask_zc_dir获取当前工作坐标系Z轴方向，作为拉伸方向。
> 4. 创建拉伸截面：函数create_dumb_curve_section根据选择的曲线创建拉伸截面。
> 5. 执行拉伸：函数do_it实现拉伸操作，包括初始化拉伸数据、设置拉伸方向、创建拉伸截面，然后调用UF_MODL_mswp_create_extrude函数进行拉伸，并释放资源。
> 6. 程序入口：ufusr是程序的入口函数，用于初始化和调用do_it。
> 7. 卸载函数：ufusr_ask_unload用于卸载程序。
>
> 综上，这段代码实现了在NX CAD系统中选择曲线进行拉伸的功能。主要流程包括选择曲线、确定拉伸方向、创建拉伸截面，然后执行拉伸操作。代码结构清晰，包含错误处理和资源释放，符合良好的编程规范。
>
