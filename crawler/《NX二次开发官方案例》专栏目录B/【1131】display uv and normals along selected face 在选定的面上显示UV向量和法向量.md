### 【1131】display uv and normals along selected face 在选定的面上显示UV向量和法向量

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** display_uv_and_normals_along_selected_face.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 里海译:Include files */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_evalsf.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_point.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_NCHARS+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_NCHARS, format, args);  
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
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_colored_conehead(double origin[3], double vec[3], UF_DISP_color_name_t color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        UF_CALL(UF_DISP_ask_closest_color_in_displayed_part(color, &attrb.color));  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        tag_t  
            part,  
            face_tag = NULL_TAG;  
        double  
            face_uv[2],  
            face_pnt[3],  
            modl_uv_min_max[4],  
            eval_uv_min_max[4],  
            unit_norm[3],  
            box[6],  
            junk[3],  
            radius,  
            rad_data,  
            U_period, V_period;  
        int  
            face_type,  
            norm_dir,  
            U_status, V_status,  
            topo_type,  
            nu, nv;  
        UF_EVALSF_p_t  
            eval;  
        UF_MODL_SRF_VALUE_t  
            surf_eval;  
        char *sPeriodStatus[] = {  
            "UF_MODL_NON_PERIODIC",   
            "UF_MODL_PERIODIC",   
            "UF_MODL_BOTH_X_PERIODIC",  
            "UF_MODL_MIN_X_PERIODIC",  
            "UF_MODL_MAX_X_PERIODIC"};  
        char *sTopoType[] = {  
            "TOPO ERROR",  
            "UF_MODL_FLAT_TOPOLOGY",  
            "UF_MODL_CYLINDRICAL_TOPOLOGY",  
            "UF_MODL_CONICAL_TOPOLOGY",  
            "UF_MODL_SPHERICAL_TOPOLOGY",  
            "UF_MODL_TOROIDAL_TOPOLOGY"};  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 里海译:Failed to initialize翻译为：初始化失败。 */  
            return;  
        }  
        /* TODO: Add your application code here 里海译:将"TODO: Add your application code here"翻译为中文为"TODO: 在这里添加你的应用程序代码"。 */  
        part = UF_ASSEM_ask_work_part();  
        while ((face_tag = select_a_face("Select a face")) != NULL_TAG)  
        {  
            ECHO("\nFace=%d\n", face_tag);  
            UF_CALL( UF_MODL_ask_face_data(face_tag, &face_type, face_pnt, unit_norm, box, &radius, &rad_data, &norm_dir) );  
            ECHO("Normal Direction (1=same as surface normal, -1=inversed)=%d\n", norm_dir);  
            UF_CALL(UF_MODL_ask_face_topology(face_tag, &topo_type));  
            ECHO("Topological type=%d / %s\n", topo_type, sTopoType[topo_type]);  
            UF_CALL( UF_MODL_ask_face_periodicity(face_tag,   
                &U_status, &U_period, &V_status, &V_period) );  
            ECHO("u_status=%d / %s u_period=%f\n", U_status, sPeriodStatus[abs(U_status)], U_period);  
            ECHO("v_status=%d / %s v_period=%f\n", V_status, sPeriodStatus[abs(V_status)], V_period);  
            UF_CALL( UF_MODL_ask_face_uv_minmax(face_tag, modl_uv_min_max) );  
            ECHO("MODL Umin=%.4f Umax=%.4f Vmin=%.4f Vmax=%.4f\n", modl_uv_min_max[0], modl_uv_min_max[1], modl_uv_min_max[2], modl_uv_min_max[3]);  
            ECHO("MODL UDelta=%.4f VDelta=%.4f\n", modl_uv_min_max[1]-modl_uv_min_max[0], modl_uv_min_max[3]-modl_uv_min_max[2]);  
            UF_CALL(UF_EVALSF_initialize_2(face_tag, &eval));  
            UF_CALL(UF_EVALSF_ask_face_uv_minmax(eval, eval_uv_min_max));  
            ECHO("EVAL Umin=%.4f Umax=%.4f Vmin=%.4f Vmax=%.4f\n", eval_uv_min_max[0], eval_uv_min_max[1], eval_uv_min_max[2], eval_uv_min_max[3]);  
            ECHO("EVAL UDelta=%.4f VDelta=%.4f\n", eval_uv_min_max[1]-eval_uv_min_max[0], eval_uv_min_max[3]-eval_uv_min_max[2]);  
            // Display U direction in RED  
            // Get point on Umax/Vmin  
            face_uv[0] = modl_uv_min_max[1];   
            face_uv[1] = modl_uv_min_max[2];  
            UF_CALL(UF_MODL_ask_face_props(face_tag, face_uv, face_pnt, junk, junk, junk, junk, junk, junk));  
            unit_norm[0] = face_pnt[0];   
            unit_norm[1] = face_pnt[1];   
            unit_norm[2] = face_pnt[2];  
            // Get point on Umin/Vmin  
            face_uv[0] = modl_uv_min_max[0];   
            face_uv[1] = modl_uv_min_max[2];  
            UF_CALL(UF_MODL_ask_face_props(face_tag, face_uv, face_pnt, junk, junk, junk, junk, junk, junk));  
            // Create vector between both points  
            unit_norm[0] = unit_norm[0]-face_pnt[0];   
            unit_norm[1] = unit_norm[1]-face_pnt[1];   
            unit_norm[2] = unit_norm[2]-face_pnt[2];  
            display_colored_conehead(face_pnt, unit_norm, UF_DISP_RED_NAME);  
            // Display V direction in GREEN  
            // Get point on Umin/Vmax  
            face_uv[0] = modl_uv_min_max[0];   
            face_uv[1] = modl_uv_min_max[3];  
            UF_CALL(UF_MODL_ask_face_props(face_tag, face_uv, face_pnt, junk, junk, junk, junk, junk, junk));  
            unit_norm[0] = face_pnt[0];   
            unit_norm[1] = face_pnt[1];   
            unit_norm[2] = face_pnt[2];  
            // Get point on Umin/Vmin  
            face_uv[0] = modl_uv_min_max[0];   
            face_uv[1] = modl_uv_min_max[2];  
            UF_CALL(UF_MODL_ask_face_props(face_tag, face_uv, face_pnt, junk, junk, junk, junk, junk, junk));  
            // Create vector between both points  
            unit_norm[0] = unit_norm[0]-face_pnt[0];   
            unit_norm[1] = unit_norm[1]-face_pnt[1];   
            unit_norm[2] = unit_norm[2]-face_pnt[2];  
            display_colored_conehead(face_pnt, unit_norm, UF_DISP_GREEN_NAME);  
            // Display normal vectors along the face in UV directions  
            // Please (un-)comment ECHO lines for detailed outputs  
            for( nu=0; nu<=5; nu++)  
            {  
                for( nv=0; nv<=5; nv++)  
                {  
                    face_uv[0] = modl_uv_min_max[0] + nu*(modl_uv_min_max[1]-modl_uv_min_max[0])/5;  
                    face_uv[1] = modl_uv_min_max[2] + nv*(modl_uv_min_max[3]-modl_uv_min_max[2])/5;  
                    // Using UF_MODL in CYAN  
                    UF_CALL(UF_MODL_ask_face_props(face_tag, face_uv, face_pnt, junk, junk, junk, junk, unit_norm, junk));  
                    display_colored_conehead(face_pnt, unit_norm, UF_DISP_CYAN_NAME);  
                    display_temporary_point(face_pnt);  
                    // Using UF_EVALSF in BLUE  
                    face_uv[0] = eval_uv_min_max[0] + nu*(eval_uv_min_max[1]-eval_uv_min_max[0])/5;  
                    face_uv[1] = eval_uv_min_max[2] + nv*(eval_uv_min_max[3]-eval_uv_min_max[2])/5;  
                    UF_CALL(UF_EVALSF_evaluate(eval, UF_MODL_EVAL_ALL, face_uv, &surf_eval));  
                    // increment the position a bit to be visibile next to the first conehead  
                    face_pnt[0] += 0.001; face_pnt[1] += 0.001; face_pnt[2] += 0.001;   
                    display_colored_conehead(face_pnt, surf_eval.srf_unormal, UF_DISP_BLUE_NAME);  
                    //ECHO("\n MODL U=%.4f V=%.4f\n", face_uv[0], face_uv[1]);  
                    //ECHO("  Px=%.4f Py=%.4f Pz=%.4f\n", face_pnt[0], face_pnt[1], face_pnt[2]);  
                    //ECHO("  MUx=%.4f MUy=%.4f MUz=%.4f\n", unit_norm[0], unit_norm[1], unit_norm[2]);  
                    //ECHO("\n EVAL U=%.4f V=%.4f\n", face_uv[0], face_uv[1]);  
                    //ECHO("  SUx=%.4f SUy=%.4f SUz=%.4f\n", surf_eval.srf_unormal[0], surf_eval.srf_unormal[1], surf_eval.srf_unormal[2]);  
                }  
            }  
            UF_CALL(UF_EVALSF_free(&eval));  
        }  
        /* Terminate the API environment 里海译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，其主要功能是沿着用户选择的曲面显示U方向、V方向和法线方向。具体来说，它实现了以下功能：
>
> 1. 使用UF_MODL模块获取曲面的参数信息，包括U方向、V方向的范围和周期性等。
> 2. 使用UF_EVALSF模块评估曲面上的点，并获取该点的法线向量。
> 3. 在曲面上沿着U方向和V方向均匀取点，并使用锥形头部表示U方向和V方向。
> 4. 在每个点处显示法线向量，使用不同颜色表示。
> 5. 在每个点处显示一个小点，帮助观察锥形头部和法线向量。
> 6. 用户可以重复选择曲面，程序会显示所选曲面的相关信息。
> 7. 程序提供了错误处理和日志功能。
> 8. 提供卸载函数，以在需要时卸载应用程序。
>
> 通过这个程序，用户可以直观地查看曲面的U方向、V方向和法线方向，对于曲面分析非常有帮助。
>
