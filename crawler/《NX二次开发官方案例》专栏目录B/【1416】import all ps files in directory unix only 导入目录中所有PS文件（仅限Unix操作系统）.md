### 【1416】import all ps files in directory unix only 导入目录中所有PS文件（仅限Unix操作系统）

#### 代码

```cpp
    /*HEAD IMPORT_ALL_PS_FILES_IN_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <Xm/FileSB.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ui_xt.h>  
    #include <uf_ps.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    typedef struct  
    {  
        char  
            cue[81],  
            title[81],  
            directory[133],  
            filter[31],  
            filename[133];  
    } FSB_data_t;  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是在 V18 版本中新增的功能。 */  
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
    static logical ask_filename(FSB_data_t *BoxData)  
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
        if (strcmp(BoxData->filename, ""))  
            return TRUE;  
        else  
            return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            cnt = 0;  
        uf_list_p_t  
            body_list;  
        char  
            fname[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1],  
            messg[150];  
        FSB_data_t  
            BoxData = { "Choose Parasolid directory", "Import Parasolid Files",  
            "", "", "" };  
        if (ask_filename(&BoxData))  
        {  
            uc4508(BoxData.filename, (1<<11)|(1<<13), 0, "*.xmt_*");  
            while (!uc4518())  
            {  
                uc4600(fname);  
                sprintf(fspec, "%s/%s", BoxData.filename, fname);  
                sprintf(messg, "(%d) Importing from %s...", ++cnt, fname);  
                printf("%s\n", messg);  
                UF_UI_set_status(messg);  
                if (!UF_CALL(UF_PS_import_data(fspec, &body_list)))  
                    UF_CALL(UF_MODL_delete_list(&body_list));  
                else  
                    break;  
            }  
            UF_CALL(uc4548());  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要实现了以下功能：
>
> 1. 文件选择对话框：使用Xt和Xm创建了一个文件选择对话框，用户可以在NX环境中选择要导入的Parasolid文件。文件对话框的标题、目录、过滤条件等都可以通过结构体FSB_data_t进行配置。
> 2. 文件导入：当用户在文件对话框中选择文件并点击确定后，会调用UF_PS_import_data函数将选中的Parasolid文件导入当前NX部件。导入前会打印导入进度信息。
> 3. 错误处理：使用UF_get_fail_message和UF_print_syslog等函数来获取并打印导入过程中的错误信息。如果发生错误，会在对话框中显示错误信息。
> 4. UF初始化和终止：在ufusr函数中对UF进行初始化和终止，以确保正确地使用UF API。
> 5. 用户询问卸载：在ufusr_ask_unload函数中返回UF_UNLOAD_IMMEDIATELY，表示用户询问是否卸载时，立即卸载当前组件。
>
> 总的来说，这段代码通过文件选择对话框实现了在NX环境中导入Parasolid文件的功能，同时提供了完善的错误处理机制。
>
