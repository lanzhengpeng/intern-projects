### 【0515】confirm exit 确认退出

#### 代码

```cpp
    static logical confirm_exit(void)  
    {  
        int  
            ii,  
            n,  
            resp;  
        tag_t  
            part;  
        char  
            *message[2] = { "You have open files that have been modified.",  
                            "Do you really want to exit?" };  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, NULL, NULL, NULL, 1, 0, 2 };  
        n = UF_PART_ask_num_parts();  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            if (UF_PART_is_modified(part)) break;  
        }  
        if (ii == n)  
        {  
            UF_CALL(UF_UI_message_dialog("Exit", UF_UI_MESSAGE_QUESTION,  
                &message[1], 1, TRUE, &buttons, &resp));  
        }  
        else  
        {  
            UF_CALL(UF_UI_message_dialog("Exit", UF_UI_MESSAGE_QUESTION,  
                message, 2, TRUE, &buttons, &resp));  
        }  
        return (2 - resp);  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发中的确认退出功能。代码逻辑如下：
>
> 1. 首先，使用UF_PART_ask_num_parts()函数获取当前打开的部件数量。
> 2. 接着，遍历每个部件，使用UF_PART_is_modified()函数检查部件是否被修改。
> 3. 如果所有部件都未被修改，则弹出一个包含“Do you really want to exit?”的对话框，询问用户是否确认退出。这个对话框有一个Yes按钮和一个No按钮。
> 4. 如果有部件被修改，则弹出一个包含“Do you really want to exit?”和“You have open files that have been modified.”的对话框，询问用户是否确认退出。这个对话框有三个按钮：Yes、No和Cancel。
> 5. 用户点击按钮后，根据按钮返回的值判断用户的选择，并返回相应的结果：Yes返回1No返回0Cancel返回-1
> 6. Yes返回1
> 7. No返回0
> 8. Cancel返回-1
> 9. confirm_exit()函数返回上述结果，以供调用者根据返回值决定后续操作。
>
> 总的来说，这段代码实现了在退出NX前，根据当前打开的部件是否被修改，弹出一个不同的确认退出对话框，并返回用户的选择结果。
>
