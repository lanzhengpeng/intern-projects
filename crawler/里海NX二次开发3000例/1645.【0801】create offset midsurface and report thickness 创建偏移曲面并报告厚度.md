### 【0801】create offset midsurface and report thickness 创建偏移曲面并报告厚度

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_sf.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp, &cnt, faces));  
        objs = *faces;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static tag_t create_offset_midsrf_feature(tag_t seed_face, tag_t feat)  
    {  
        double  
            cliff_angle = 5.0,  // best practice = 5, max suggested = 15  
            percentage_dist = 50.0;  
        UF_CALL(UF_SF_create_offset_midsrf(seed_face, cliff_angle, percentage_dist,  
                                           &feat));  
        return feat;  
    }  
    static void report_offset_midsrf_thickness( tag_t feat_midsrf)  
    {  
        double  
            thickness;  
        char  
            msg[300] = { "" };  
        UF_SF_ask_offset_midsrf_thickness(feat_midsrf, &thickness);  
        ECHO( "thickness of offset midsurface: %f\n", thickness);  
    }  
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *faces,  
            feat=NULL_TAG;  
        while ((n = select_faces("Select faces", &faces)) != 0)  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                ECHO( "seed face: %d\n", faces[ii]);  
                feat = create_offset_midsrf_feature(faces[ii], feat);  
                if (feat != NULL_TAG)  
                    report_offset_midsrf_thickness(feat);  
            }  
            UF_free(faces);  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 选择面：代码提供了mask_for_faces函数，用于设置选择模式，只选择面特征。同时，select_faces函数实现了与用户交互选择面的功能。
> 2. 偏置中面特征：create_offset_midsrf_feature函数用于创建偏置中面特征，给定种子面和偏置角度，可以创建出偏置中面特征。
> 3. 查询偏置厚度：report_offset_midsrf_thickness函数用于查询偏置中面特征的厚度。
> 4. 主要逻辑：do_it函数实现了选择面、创建偏置中面特征、打印厚度的循环。
> 5. 接口函数：ufusr函数作为NX的二次开发接口函数，用于初始化和执行do_it函数。
> 6. 错误处理：代码中使用了宏UF_CALL进行错误处理，并在出错时打印错误信息。
> 7. 清理函数：ufusr_ask_unload函数用于卸载二次开发程序。
>
> 总体来看，这段代码实现了在NX中选择面，并为其创建偏置中面特征的功能，同时提供了查询偏置厚度的功能。通过使用NX的二次开发接口，实现了交互式的偏置中面特征创建。
>
