### 【2091】report face info 询问面信息，报告面数据，报告面属性，报告面环

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_face_data(tag_t face, double center[3])  
    {  
        int  
            norm_dir,  
            type;  
        double  
            dir[3],  
            box[6],  
            radius,  
            rad_data;  
        UF_CALL(UF_MODL_ask_face_data(face, &type, center, dir, box,  
            &radius, &rad_data, &norm_dir));  
        ECHO("\nFace = %d\n", face);  
        ECHO("  type = %d\n", type);  
        ECHO("  center = %f, %f, %f\n", center[0], center[1], center[2]);  
        ECHO("  dir = %f, %f, %f\n", dir[0], dir[1], dir[2]);  
        ECHO("  box = %f, %f, %f\n        %f, %f, %f\n",  
            box[0], box[1], box[2], box[3], box[4], box[5]);  
        ECHO("  radius = %f\n", radius);  
        ECHO("  rad_data = %f\n", rad_data);  
        ECHO("  norm_dir = %d\n", norm_dir);  
    }  
    static void report_face_props(tag_t face, double center[3])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU1[3],  
            dirU2[3],  
            dirV1[3],  
            dirV2[3],  
            norm_dir[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, center, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, dirU1, dirV1, dirU2,  
            dirV2, norm_dir, radii));  
        ECHO("\n  parm = %f, %f\n", parm[0], parm[1]);  
        ECHO("  pnt = %f, %f, %f\n", pnt[0], pnt[1], pnt[2]);  
        ECHO("  dirU1 = %f, %f, %f\n", dirU1[0], dirU1[1], dirU1[2]);  
        ECHO("  dirV1 = %f, %f, %f\n", dirV1[0], dirV1[1], dirV1[2]);  
        ECHO("  dirU2 = %f, %f, %f\n", dirU2[0], dirU2[1], dirU2[2]);  
        ECHO("  dirV2 = %f, %f, %f\n", dirV2[0], dirV2[1], dirV2[2]);  
        ECHO("  norm_dir = %f, %f, %f\n", norm_dir[0], norm_dir[1], norm_dir[2]);  
        ECHO("  radii = %f, %f\n", radii[0], radii[1]);  
    }  
    static void report_face_loops(tag_t face)  
    {  
        char  
            *loop_types[4] = { "", "Peripheral", "Hole", "Other" };  
        int  
            j,k,  
            loop_type,  
            num_edg_obj,  
            num_loops = 0,  
            objtyp,  
            objsub;  
        tag_t  
            edge_obj;  
        uf_list_p_t  
            edge_list;  
        uf_loop_p_t  
            loop_list;  
        UF_CALL(UF_MODL_ask_face_loops(face, &loop_list));  
        if (loop_list)  
            UF_CALL(UF_MODL_ask_loop_list_count(loop_list, &num_loops));  
        ECHO("\n  face has %d loops\n", num_loops);  
        for( k=0; k<num_loops; k++ )  
        {  
            UF_CALL(UF_MODL_ask_loop_list_item(loop_list,k,&loop_type,&edge_list));  
            UF_CALL(UF_MODL_ask_list_count(edge_list, &num_edg_obj));  
            ECHO("    %d.  %s loop has %d curves\n", k+1, loop_types[loop_type],  
                num_edg_obj);  
            for( j=0; j<num_edg_obj; j++ )  
            {  
                UF_CALL(UF_MODL_ask_list_item(edge_list, j, &edge_obj));  
                UF_CALL(UF_MODL_create_curve_from_edge(edge_obj,&edge_obj));  
                UF_CALL(UF_OBJ_ask_type_and_subtype(edge_obj, &objtyp, &objsub));  
                ECHO("        %d.  curve type/subtype is %d/%d\n", j+1, objtyp,  
                    objsub);  
                UF_CALL(UF_OBJ_delete_object(edge_obj));  
            }  
        }  
        UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
    }  
    static void do_it(void)  
    {  
        double  
            center[3];  
        tag_t  
            face;  
        while ((face = select_a_face("Select a face")) != NULL_TAG)  
        {  
            report_face_data(face, center);  
            report_face_props(face, center);  
            report_face_loops(face);  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是选择一个面，并报告该面的数据、属性和环信息。
>
> 1. 包含了必要的NX头文件，如uf.h等。
> 2. 定义了ECHO宏，用于输出信息到日志窗口。
> 3. 定义了UF_CALL宏，用于调用NX API函数，并报告错误。
> 4. 实现了mask_for_faces函数，用于设置选择过滤条件为任意实体上的任意面。
> 5. 实现了select_a_face函数，用于选择一个面并返回其tag。
> 6. 实现了report_face_data函数，用于报告面的数据，如类型、中心点、法线方向等。
> 7. 实现了report_face_props函数，用于报告面的参数、点、方向等信息。
> 8. 实现了report_face_loops函数，用于报告面包含的环信息。
> 9. 实现了do_it函数，用于循环选择面，并报告其信息。
> 10. 实现了ufusr函数，作为程序的入口，初始化NX环境，调用do_it，然后终止NX环境。
> 11. 实现了ufusr_ask_unload函数，用于在程序卸载时立即卸载。
>
> 这段代码通过选择一个面，并调用NX的API获取和输出该面的各种信息，展示了NX二次开发的基本流程。
>
