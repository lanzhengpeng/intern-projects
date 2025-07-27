### 【0794】create note 创建注释

#### 代码

```cpp
    /*HEAD CREATE_NOTE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_ugfont.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中的描述，UF_print_syslog 是 V18 版本中新增的函数。因此，我的翻译是：

注意：UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static tag_t ask_font_table_entity(void)  
    {  
        tag_t  
            fte = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));  
        return (fte);  
    }  
    static int ask_font_index_number(char *font_name)  
    {  
        int  
            index;  
        tag_t  
            fte = ask_font_table_entity();  
        UF_CALL(UF_UGFONT_add_font(fte, &index, font_name));  
        return (index);  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static tag_t set_wcs_to_current_view(void)  
    {  
        tag_t  
            csys,                       /* Csys object  译:Csys对象是一个用于存储和操作坐标系数据的类。它包括坐标原点、坐标轴方向、坐标轴单位等属性，并提供了平移、旋转、缩放等变换操作的方法。通过Csys对象，可以方便地在三维空间中定义和操作坐标系。 */  
            matobj,                     /* Matrix object 译:Matrix 对象 */  
            orig_wcs;  
        double  
            origin[3],                  /* View's center 译:View的center翻译为视图的中心。 */  
            matrix[9],                  /* View's matrix 译:视图的变换矩阵 */  
            scale;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        if (!UF_CALL(UF_CSYS_create_matrix(matrix, &matobj)) &&  
            !UF_CALL(UF_CSYS_create_temp_csys(origin, matobj, &csys)))  
            UF_CALL(UF_CSYS_set_wcs(csys));  
        return orig_wcs;  
    }  
    static void set_wcs_origin(double new_origin[3])  
    {  
        tag_t  
            orig_wcs,  
            wcs,  
            wcs_mx;  
        double  
            old_origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(orig_wcs, &wcs_mx, old_origin));  
        UF_CALL(UF_CSYS_create_temp_csys(new_origin, wcs_mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            old_wcs,  
            the_note;  
        char  
            *text[1] = { "UF_DRF_create_note"};  
        double  
            origin[3],  
            zero[3] = { 0, 0, 0 };  
        UF_DRF_lettering_preferences_t  
            lettering_preferences;  
        UF_CALL(UF_DRF_ask_lettering_preferences(&lettering_preferences));  
        lettering_preferences.general_text.cfw.font =  
            ask_font_index_number("leroy");  
        UF_CALL(UF_DRF_set_lettering_preferences(&lettering_preferences));  
        while (specify_position("create note", origin))  
        {  
            old_wcs = set_wcs_to_current_view();  
            set_wcs_origin(origin);  
            UF_CALL(UF_DRF_create_note(1, text, zero, 0, &the_note));  
            UF_CALL(UF_CSYS_set_wcs(old_wcs));  
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

> 这段代码是用于在NX中创建注释的UFUN函数。下面是该代码的主要功能：
>
> 1. 包含了必要的NX Open头文件，用于二次开发。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并打印错误信息。
> 3. ask_font_table_entity函数用于获取当前部件的字体表实体。
> 4. ask_font_index_number函数用于向字体表中添加字体并获取其索引号。
> 5. specify_position函数用于提示用户输入注释位置。
> 6. set_wcs_to_current_view函数用于设置当前视图作为工作坐标系。
> 7. set_wcs_origin函数用于设置工作坐标原点。
> 8. do_it函数是主函数，用于循环提示用户输入注释位置，并调用UF_DRF_create_note在指定位置创建注释。
> 9. ufusr是NX调用的主函数，用于初始化、调用do_it函数创建注释，然后终止。
> 10. ufusr_ask_unload函数用于设置卸载类型。
>
> 总体来说，该代码实现了在NX中根据用户指定位置创建注释的功能。
>
