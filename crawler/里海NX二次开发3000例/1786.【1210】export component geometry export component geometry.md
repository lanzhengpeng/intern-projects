### 【1210】export component geometry export component geometry

#### 代码

```cpp
    /*HEAD EXPORT_COMPONENT_GEOMETRY CCC UFUN */  
    /*  In V13.0 most of the code in this program is obsolete.  Object  
        occurrences are directly exportable, so the tricks here are  
        unnessary.  Also look for a new function UF_UI_create_filebox.  */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <unistd.h>  
    #include <Xm/FileSB.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_group.h>  
    #include <uf_ui.h>  
    #include <uf_ui_xt.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_trns.h>  
    #include <uf_cfi.h>  
    #define W_OK    2  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是在V18中新引入的，只回答译文，不要废话。 */  
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
    typedef struct  
    {  
        char  
            cue[81],  
            title[81],  
            directory[133],  
            filter[31],  
            filename[133];  
    } FSB_data_t;  
    static void file_select_cb(Widget widget, XtPointer client_data,  
        XtPointer call_data)  
    {  
        char  
            *temp;  
        XmFileSelectionBoxCallbackStruct  
            *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;  
        FSB_data_t  
            *BoxData = (FSB_data_t *) client_data;  
        XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &temp);  
        strcpy(BoxData->filename, temp);  
        XtFree(temp);  
        XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &temp);  
        strcpy(BoxData->directory, temp);  
        XtFree(temp);  
        XmStringGetLtoR(cbs->pattern, XmFONTLIST_DEFAULT_TAG, &temp);  
        strcpy(BoxData->filter, temp);  
        XtFree(temp);  
        UF_UI_exit_dialog();  
    }  
    static void exit_cb(Widget widget, XtPointer client_data, XtPointer call_data)  
    {  
        FSB_data_t  
            *BoxData = (FSB_data_t *) client_data;  
        strcpy(BoxData->filename, "");  
        UF_UI_exit_dialog();  
    }  
    static void ask_filename(FSB_data_t *BoxData)  
    {  
        Arg  
            args[3];  
        Position  
            posX,  
            posY;  
        Widget  
            FileBox,  
            parent = UF_UI_get_default_parent();  
        UF_CALL(UF_UI_set_prompt(BoxData->cue));  
        UF_CALL(UF_UI_get_DA1_coords(&posX, &posY));  
        XtSetArg (args[0], XmNx, posX);  
        XtSetArg (args[1], XmNy, posY);  
        XtSetArg (args[2], XmNdefaultPosition, False);  
        FileBox = XmCreateFileSelectionDialog(parent, "FileBox", args, 3);  
        XtVaSetValues(XtParent(FileBox), XtVaTypedArg, XmNtitle, XmRString,  
            BoxData->title, strlen(BoxData->title)+1, NULL );  
        XtVaSetValues(FileBox, XtVaTypedArg, XmNpattern, XmRString,  
            BoxData->filter, strlen(BoxData->filter)+1, NULL );  
        XtVaSetValues(FileBox, XtVaTypedArg, XmNdirectory, XmRString,  
            BoxData->directory, strlen(BoxData->directory)+1, NULL );  
        XtAddCallback (FileBox, XmNcancelCallback, exit_cb, BoxData);  
        XtAddCallback (FileBox, XmNokCallback, file_select_cb, BoxData);  
        XtUnmanageChild(XmFileSelectionBoxGetChild(FileBox,XmDIALOG_HELP_BUTTON));  
        UF_UI_run_dialog(FileBox);  
    }  
    static logical select_components(char *prompt, tag_t **comps, int *cnt)  
    {  
        int  
            ii,  
            resp;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_component_type;  
        mask.object_subtype = 0;  
        mask.solid_type = 0;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &resp, cnt, comps));  
        if (resp == 3)  
        {  
            for (ii = 0; ii < *cnt; ii++)  
                UF_CALL(UF_DISP_set_highlight(*(*comps+ii), FALSE));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void export_comp_members(char *filename, tag_t comp, logical *new_part)  
    {  
        logical  
            trans_flag;  
        int  
            cnt = 0,  
            cp = 2,  
            ii,  
            irc,  
            subtype,  
            troff = 2,  
            type,  
            wl = -1;  
        tag_t  
            cmem = NULL_TAG,  
            *copies,  
            grp[1],  
            *members = NULL,  
            work_part,  
            x;  
        double  
            csys_matrix[9],  
            origin[3],  
            *trans,  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_PART_export_options_t  
            export_options;  
        trans = &transform[0][0];  
        while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(comp, &cmem)) &&  
               (cmem != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(cmem, &type, &subtype));  
            if (type == UF_component_type)  
                export_comp_members(filename, cmem, new_part);  
            else if( ((type == 70) && (subtype == 0)) || (type <= 24))  
            {  
                if (cnt - (cnt/10) == 0)  
                    members = (tag_t *)realloc(members, (cnt + 10) * sizeof(tag_t));  
                members[cnt] = UF_ASSEM_ask_prototype_of_occ(cmem);  
                cnt++;  
            }  
        }  
        if (cnt > 0)  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform));  
            work_part = UF_ASSEM_ask_work_part();  
            UF_ASSEM_set_work_part(UF_ASSEM_ask_prototype_of_occ(comp));  
            if ((origin[0] != 0) || (origin[1] != 0) || (origin[2] != 0)  
            ||  (csys_matrix[0] != 1)||(csys_matrix[1] != 0)||(csys_matrix[2] != 0)  
            ||  (csys_matrix[3] != 0)||(csys_matrix[4] != 1)||(csys_matrix[5] != 0)  
            ||  (csys_matrix[6] != 0)||(csys_matrix[7] != 0)||(csys_matrix[8] != 1))  
            {  
                copies = (tag_t *)malloc(cnt * sizeof(tag_t));  
                FTN(uf5947)(trans,members,&cnt,&cp,&wl,&troff,copies,&x,&irc);  
                UF_CALL(irc);  
                trans_flag = TRUE;  
            }  
            else  
            {  
                trans_flag = FALSE;  
                copies = members;  
                for (ii = 0; ii < cnt; ii++) FTN(uf5515)(&copies[ii]);  
            }  
            UF_CALL(UF_GROUP_create_group(copies, cnt, grp));  
            UF_CALL(uc5029(grp[0], instance_name, strlen(instance_name)));  
            export_options.new_part = *new_part;  
            export_options.params_mode = UF_PART_remove_params;  
            export_options.expression_mode = UF_PART_copy_exp_shallowly;  
            UF_CALL(UF_PART_export_with_options(filename, 1, grp, &export_options));  
            *new_part = FALSE;  
            if (trans_flag)  
            {  
                UF_CALL(UF_GROUP_ungroup_top(grp[0]));  
                for (ii = 0; ii < cnt; ii++) UF_OBJ_delete_object(copies[ii]);  
                free(copies);  
            }  
            free(members);  
            UF_ASSEM_set_work_part(work_part);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            new_part;  
        int  
            cnt,  
            ii,  
            save_emphasize;  
        tag_t  
            *comps;  
        FSB_data_t  
            BoxData = { "Exported part", "Part Name", "", "*.prt", "" };  
        UF_ASSEM_options_t  
            assem_options;  
        do  
        {  
            ask_filename(&BoxData);  
            if (strcmp(BoxData.filename, ""))  
            {  
                if (select_components("Select components to export", &comps, &cnt))  
                {  
                    UF_DISP_set_display(FALSE);  
                    UF_CALL(UF_ASSEM_ask_assem_options(&assem_options));  
                    save_emphasize = assem_options.emphasize;  
                    if (save_emphasize != 1)  
                    {  
                        assem_options.emphasize = 1;  
                        UF_CALL(UF_ASSEM_set_assem_options(&assem_options));  
                    }  
                    new_part = access(BoxData.filename, W_OK);  
                    for (ii = 0; ii < cnt; ii++)  
                        export_comp_members(BoxData.filename, comps[ii], &new_part);  
                    UF_free(comps);  
                    if (save_emphasize != 1)  
                    {  
                        assem_options.emphasize = save_emphasize;  
                        UF_CALL(UF_ASSEM_set_assem_options(&assem_options));  
                    }  
                    UF_DISP_set_display(TRUE);  
                }  
            }  
        } while (strcmp(BoxData.filename, ""));  
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

> 这段NX二次开发代码的主要功能是导出组件几何体。下面是代码的主要功能介绍：
>
> 1. 定义了一个结构体FSB_data_t，用于存储文件选择对话框的数据。
> 2. 定义了文件选择回调函数file_select_cb，用于获取用户选择的文件名、目录和过滤器。
> 3. 定义了退出回调函数exit_cb，用于清空文件名并关闭对话框。
> 4. 定义了ask_filename函数，用于创建文件选择对话框并获取用户输入的文件名。
> 5. 定义了select_components函数，用于让用户选择要导出的组件，并获取组件的tag。
> 6. 定义了export_comp_members函数，用于递归导出组件的所有成员几何体到指定文件。如果组件有位置变换，会先平移组件，再导出几何体。
> 7. 在do_it函数中，首先调用ask_filename获取要导出的文件名，然后调用select_components让用户选择组件，接着调用export_comp_members递归导出每个组件的几何体。
> 8. ufusr是NX调用的主函数，其中调用do_it函数完成组件几何体的导出。
> 9. ufusr_ask_unload用于设置二次开发程序的卸载模式。
>
> 总体来说，这段代码实现了NX组件几何体的导出功能，通过文件对话框获取文件名，让用户选择要导出的组件，递归导出每个组件的几何体到指定文件。
>
