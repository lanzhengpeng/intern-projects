### 【1649】move body to place selected face on absolute xy plane 移动体，使选定面位于绝对xy平面上

#### 代码

```cpp
    // First tested in NX 11.0.2  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_mtx.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
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
    /* qq3123197280 */  
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
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_face_midpoint(tag_t face, double mid_point[3])  
    {  
        double  
            norm[3],  
            parm[2],  
            radii[2],  
            u1[3],  
            v1[3],  
            u2[3],  
            v2[3],  
            uvs[4];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = (uvs[0] + uvs[1]) / 2.;  
        parm[1] = (uvs[2] + uvs[3]) / 2.;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, mid_point, u1, u2, v1, v2,  
            norm, radii));  
    }  
    static tag_t set_wcs_by_face_u_and_normal_at_point(tag_t face, double origin[3])  
    {  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        double  
            csys[9],  
            junk[3],  
            mag,  
            parm[2],  
            pnt[3],  
            tol,  
            x_dir[3],  
            y_dir[3],  
            z_dir[3];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(UF_MODL_ask_face_parm(face, origin, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir,   
            junk, junk, junk, z_dir, junk));  
        // Reverse the z_dir, which should be the face normal...  
        UF_VEC3_negate(z_dir, z_dir);  
        UF_VEC3_cross(z_dir, x_dir, y_dir);  
        UF_CALL(UF_VEC3_unitize(x_dir, tol, &mag, &csys[0]));  
        UF_CALL(UF_VEC3_unitize(y_dir, tol, &mag, &csys[3]));  
        UF_CALL(UF_VEC3_unitize(z_dir, tol, &mag, &csys[6]));  
        UF_MTX3_ortho_normalize(csys);  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return wcs;  
    }  
    static void matrix_csys2csys(tag_t ref_csys, tag_t dest_csys, double mx[12])  
    {  
        double  
            csys[9],  
            orig[3],  
            tx1[12],  
            tx2[12],  
            tx3[12],  
            tx4[12],  
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
        /* set up to translate from reference csys back to absolute 里海译:从参考坐标系转换回绝对坐标系的设置。 */  
        for (ii = 0; ii < 3; ii++) v[ii] = -orig[ii];  
        FTN(uf5943)(v, tx2);  
        /* combine this with the rotation matrix from the reference csys 里海译:将这个与参考坐标系的旋转矩阵结合 */  
        FTN(uf5942)(tx2, tx1, tx3);  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));  
        /* Invert the rotation from the destination matrix 里海译:反转目标矩阵的旋转 */  
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
        /* set up to translate from abs to the destination csys 里海译:设置从ABS坐标系转换到目标坐标系的转换。 */  
        FTN(uf5943)(orig, tx1);  
        /* combine this with the inverted rotation csys above 里海译:结合上面倒置的旋转坐标系。 */  
        FTN(uf5942)(tx2, tx1, tx4);  
        /* and the one from the reference csys 里海译:参考坐标系中的那个 */  
        FTN(uf5942)(tx3, tx4, mx);  
    }  
    static void do_it(void)  
    {  
        int  
            one = 1,  
            two = 2,  
            zero = 0,  
            irc = 0;  
        tag_t  
            theFace = NULL_TAG,  
            theBody = NULL_TAG,  
            abs_mx = NULL_TAG,  
            abs_id = NULL_TAG,  
            objects[1] = { NULL_TAG },  
            grp = NULL_TAG,  
            originalWCS = NULL_TAG,  
            tempWCS = NULL_TAG;  
        double  
            matrix[9] = { 0., 0., 0., 0., 0., 0., 0., 0., 0. },  
            zeropt[3] = { 0., 0., 0. },  
            faceMidPt[3] = { 0., 0., 0. },  
            mx[12] = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };  
        UF_CALL(UF_CSYS_ask_wcs(&originalWCS));  
        theFace = select_a_face("Select a Face:");  
        if (NULL_TAG == theFace) return;  
        UF_CALL(UF_MODL_ask_face_body(theFace, &theBody));  
        // Get centerpoint of the face  
        ask_face_midpoint(theFace, faceMidPt);  
        // temporarily move the WCS to the center of the face  
        tempWCS = set_wcs_by_face_u_and_normal_at_point(theFace, faceMidPt);  
        // perform the translation of the body  
        UF_MTX3_identity(matrix);  
        UF_CALL(UF_CSYS_create_matrix(matrix, &abs_mx));  
        UF_CALL(UF_CSYS_create_temp_csys(zeropt, abs_mx, &abs_id));  
        matrix_csys2csys(tempWCS, abs_id, mx);  
        objects[0] = theBody;  
        FTN(uf5947)(mx, objects, &one, &one, &zero, &two, NULL, &grp, &irc);  
        UF_MODL_update();  
        UF_CALL(UF_VIEW_fit_view(NULL_TAG, 0.9));  
        /***********************************************************************  
           Possible values for the irc variable -  
           create a switch-case statement if desired to test them.  
        0 - Success  
        1 - Too Many Or Too Few Entities To  
        Transform  
        2 - Invalid Parameter  
        3 - Invalid Object (Not Alive Or Not Transformable)  
        4 - Shear Matrix With Non-shearable Objects In The List  
        5 - Modeling update error  
        6 - Error adding modeling data to copied solid  
        7 - Cannot scale parametric solid  
        8 - Transformation is unsuitable for the object  
        9 - Cannot scale developed curve  
        10 - Cannot mirror solid  
        11 - Cannot transform occurrence  
        12 - Cannot transform sketches or sketch curves  
        ***************************************************************/  
        // revert to the original WCS  
        UF_CALL(UF_CSYS_set_wcs(originalWCS));  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_TEXT_set_text_mode(UF_TEXT_ALL_UTF8));  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，其主要功能包括：
>
> 1. 用户选择面：首先提示用户选择一个面，并获取该面的信息，如中心点和法线方向。
> 2. 坐标系设置：根据面的中心点和法线方向，建立一个坐标系，并将该坐标系设置为当前坐标系。
> 3. 变换：然后对所选面所在的实体进行变换，将其移动到原点，并保持其方向不变。
> 4. 坐标系恢复：变换完成后，恢复到原始的坐标系。
> 5. 错误处理：代码中包含了错误处理，可以捕捉函数调用错误并给出提示信息。
> 6. NX接口调用：代码调用了NX提供的几何建模、坐标系操作、变换等接口。
> 7. 用户交互：通过UF_UI接口进行用户交互，提示用户选择面。
> 8. 日志输出：通过UF_SYSLOG接口输出日志信息。
> 9. 模块封装：代码结构清晰，将主要逻辑封装在do_it函数中。
>
> 总体来说，这段代码通过NX提供的二次开发接口，实现了选择一个面，并将其移动到坐标系原点的功能。
>
