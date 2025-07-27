### 【1923】report all hole data 报告所有孔洞数据

#### 代码

```cpp
    /*HEAD REPORT_ALL_HOLE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <stdarg.h>  
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
    static int report_error( char *file, int line, char *call, int irc) {  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void report_hole_data(tag_t body)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            feat;  
        char  
            *feat_type;  
        double  
            ijk[3],  
            loc[3],  
            mag,  
            x_dir[3],  
            y_dir[3];  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        UF_CALL(UF_MODL_ask_list_count(feat_list, &n_feats));  
        for (ii=0; ii < n_feats; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(feat_list, ii, &feat));  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            if (strstr(feat_type, "HOLE") != NULL)  
            {  
                UF_CALL(UF_MODL_ask_feat_location(feat, loc));  
                UF_CALL(UF_MODL_ask_feat_direction(feat, x_dir, y_dir));  
                UF_CALL(UF_VEC3_unitize(x_dir, 0, &mag, ijk));  
                ECHO("%s at %f,%f,%f vector %f,%f,%f\n", feat_type,  
                    loc[0], loc[1], loc[2], ijk[0], ijk[1], ijk[2]);  
                UF_DISP_conehead(UF_DISP_WORK_VIEW_ONLY, loc, ijk, 0);  
            }  
            UF_free(feat_type);  
        }  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face));  
            if (face)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
                if (subtype == UF_solid_face_subtype) return face;  
            }  
        } while (face);  
        return NULL_TAG;  
    }  
    static void report_cyl_hole_data(tag_t face)  
    {  
        int  
            code,  
            irc,  
            norm_dir,  
            type;  
        double  
            box[6],  
            dir[3],  
            ijk[3],  
            mag,  
            point[3],  
            rad,  
            rad_data;  
        tag_t  
            body;  
        UF_CALL(UF_MODL_ask_face_data(face, &type, point, dir, box, &rad,  
            &rad_data, &norm_dir));  
        if (type == UF_cylinder_type && rad < 1.0) /* limit size reported 里海译:限制报告大小 */  
        {  
            UF_CALL(UF_MODL_ask_face_body(face, &body));  
            FTN(uf6541)(point, &body, &irc, &code);  
            if (code == 2)  /* point not in material, otherwise its a pad 里海译:点不在材料内，否则为垫子。 */  
            {  
                UF_CALL(UF_VEC3_unitize(dir, 0, &mag, ijk));  
                printf("cylinder at %f,%f,%f vector %f,%f,%f\n",  
                    point[0], point[1], point[2], ijk[0], ijk[1], ijk[2]);  
                UF_DISP_conehead(UF_DISP_WORK_VIEW_ONLY, point, ijk, 0);  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            item = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        /* First do the hole features 里海译:首先进行孔特征处理。 */  
        while ((item = ask_next_body(part, item)) != NULL_TAG)  
            report_hole_data(item);  
        /* Now do the "holes" that not hole features 里海译:现在做那些“洞”，那些不是洞的特征。 */  
        while ((item = ask_next_face(part, item)) != NULL_TAG)  
            report_cyl_hole_data(item);  
    }  
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

> 这段NX二次开发代码的主要功能是遍历当前显示的零件中的实体，并报告孔特征和小的圆柱面特征。
>
> 关键功能点如下：
>
> 1. 定义了一个宏UF_CALL，用于调用NX API函数，并捕获错误代码。若发生错误，则会输出错误信息。
> 2. 定义了ask_next_body函数，用于遍历零件中的下一个实体。
> 3. 定义了report_hole_data函数，用于报告实体的孔特征。它获取实体中的所有特征，并输出孔特征的位置和方向。
> 4. 定义了ask_next_face函数，用于遍历零件中的下一个面。
> 5. 定义了report_cyl_hole_data函数，用于报告小的圆柱面特征。它获取面的信息，若面是小的圆柱面，则输出其位置和方向。
> 6. do_it函数首先遍历所有实体，并报告孔特征。然后遍历所有面，并报告小的圆柱面特征。
> 7. ufusr函数是NX二次开发的入口函数。在初始化NX环境后，调用do_it函数进行特征报告，最后终止NX环境。
> 8. ufusr_ask_unload函数用于卸载用户函数。
>
> 总的来说，该代码通过遍历零件中的实体和面，实现了对孔特征和小的圆柱面特征的识别和报告，具有一定的实用价值。
>
