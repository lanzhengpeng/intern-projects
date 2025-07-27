### 【0878】create sheet udo 创建Sheet UDO

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <ctype.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_csys.h>  
    #include <uf_exit.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_view.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static UF_UDOBJ_class_t my_class_id = 0;  
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void)  
    {  
        return my_class_id;  
    }  
    DllExport extern void display_my_udo_cb(tag_t udo_tag, void *display_context)  
    {  
        int  
            ii;  
        double  
            normals[15],  
            poly_points[15];  
        UF_UDOBJ_all_data_t  
            data;  
        UF_DISP_inquire_t  
            display_info;  
        UF_DISP_facet_t  
            facets[2];  
        facets[0].vertices = poly_points;  
        facets[0].normals = normals;  
        facets[1].vertices = &poly_points[6];  
        facets[1].normals = &normals[6];  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_DISP_ask_display_context(display_context, &display_info));  
        if (display_info.is_draw_open_disp == TRUE) return;  
        UF_CALL(UF_UDOBJ_ask_udo_data (udo_tag, &data));  
        UF_VEC3_copy(&data.doubles[0], &poly_points[0]);  
        UF_VEC3_add(&poly_points[0], &data.doubles[3], &poly_points[3]);  
        UF_VEC3_add(&poly_points[3], &data.doubles[6], &poly_points[6]);  
        UF_VEC3_sub(&poly_points[6], &data.doubles[3], &poly_points[9]);  
        UF_VEC3_copy(&data.doubles[0], &poly_points[12]);  
        for (ii = 0; ii < 15; ii = ii + 3)  
            UF_VEC3_copy(&data.doubles[9], &normals[ii]);  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        UF_CALL(UF_DISP_display_polyline(poly_points, 5, display_context));  
        UF_CALL(UF_DISP_display_facets(facets, 3, 2, UF_DISP_TRIANGLE,  
            display_context));  
        UF_terminate();  
    }  
    static void register_sheet_udo(void)  
    {  
        UF_CALL(UF_UDOBJ_create_class("Sheet_UDO", "Sheet UDO", &my_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(my_class_id));  
        UF_CALL(UF_UDOBJ_register_attn_pt_cb(my_class_id, display_my_udo_cb));  
        UF_CALL(UF_UDOBJ_register_display_cb(my_class_id, display_my_udo_cb));  
        UF_CALL(UF_UDOBJ_register_fit_cb(my_class_id, display_my_udo_cb));  
        UF_CALL(UF_UDOBJ_register_select_cb(my_class_id, display_my_udo_cb));  
    }  
    /* 里海 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_sheet_udo();  
        UF_terminate();  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
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
    static tag_t cycle_by_name_prefix(char *prefix, tag_t part, tag_t object,  
        char *fullname)  
    {  
        int  
            ii;  
     /*  make sure the name is all in upper case 译:确保名称全部为大写。 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static void ask_name_min_max_suffix(char *prefix, int *min, int *max)  
    {  
        int  
            n;  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            fmt[MAX_ENTITY_NAME_SIZE + 1],  
            fullname[MAX_ENTITY_NAME_SIZE + 1];  
        sprintf(fmt, "%s%s", prefix, "%d");  
        *min = 0;  
        *max = 0;  
        while ((object = cycle_by_name_prefix(prefix, part, object, fullname))  
            != NULL_TAG)  
        {  
            sscanf(fullname, fmt, &n);  
            if ((*min == 0) || (n < *min)) *min = n;  
            if (n > *max) *max = n;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            min,  
            max;  
        tag_t  
            my_udo;  
        double  
            axes[9],  
            loc[3],  
            mag,  
            tol;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1],  
            prefix[MAX_ENTITY_NAME_SIZE+1] = { "SHEET_UDO_" };  
        UF_UDOBJ_class_t  
            sheet_class_id;  
        sheet_class_id = get_my_class_id();  
        if (sheet_class_id == 0)  
        {  
            register_sheet_udo();  
            sheet_class_id = get_my_class_id();  
        }  
        UF_MODL_ask_distance_tolerance(&tol);  
        ask_name_min_max_suffix(prefix, &min, &max);  
        while (specify_point("Indicate UDO position", loc))  
        {  
            UF_CALL(UF_UDOBJ_create_udo(sheet_class_id, &my_udo));  
            sprintf(name, "%s%d", prefix, ++max);  
            UF_CALL(UF_OBJ_set_name(my_udo, name));  
            UF_CALL(UF_OBJ_set_name_origin(my_udo, loc));  
            UF_CALL(UF_OBJ_set_color(my_udo, max%UF_OBJ_MAX_COLOR));  
            UF_CALL(UF_UDOBJ_add_doubles(my_udo, 3, loc));  
            ask_wcs_info(loc, axes);  
            for (ii = 0; ii < 9; ii = ii + 3)  
                UF_VEC3_unitize(&axes[ii], tol, &mag, &axes[ii]);  
            UF_CALL(UF_UDOBJ_add_doubles(my_udo, 9, axes));  
            UF_CALL(UF_DISP_add_item_to_display(my_udo));  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了自定义用户对象(UDO)的创建和显示。以下是代码的主要内容和步骤：
>
> 1. 错误报告和初始化函数：定义了错误报告函数report_error和初始化函数UF_CALL。
> 2. 获取UDO类的ID：定义了获取UDO类ID的函数get_my_class_id，用于获取自定义的UDO类。
> 3. 自定义UDO类的注册：通过register_sheet_udo函数创建并注册了自定义的Sheet UDO类。
> 4. UDO的创建和显示回调函数：定义了UDO的创建、显示、拟合、选择等回调函数display_my_udo_cb，用于自定义UDO的显示。
> 5. UFSTA和UFUSR函数：UFSTA函数用于在NX启动时初始化自定义UDO类，而UFUSR函数则用于创建和显示UDO。
> 6. 获取坐标系信息和名称后缀处理：ask_wcs_info函数用于获取坐标系信息，ask_name_min_max_suffix函数用于获取名称的后缀范围。
> 7. 交互式创建UDO：do_it函数用于交互式创建UDO，包括获取用户输入的位置、计算单位向量、设置UDO的名称和颜色等，并最终添加到显示列表中。
>
> 总体来说，这段代码通过自定义UDO类并注册相应的回调函数，实现了交互式创建和显示自定义UDO的功能。
>
