### 【2674】select features 选择特征

#### 代码

```cpp
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_UI_feat_sel_type_t  
            all = UF_UI_FEAT_SEL_TYPE_BROWSEABLE;  
        UF_CALL(UF_UI_select_feature(prompt, &all, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于在NX中实现特征选择的功能。其主要功能是调用UF_UI_select_feature函数，以交互方式选择特征。
>
> 具体来说：
>
> 1. 函数名为select_features，接受一个提示字符串和一个特征数组指针作为参数。
> 2. 定义了两个整型变量cnt和resp，分别用于存储选择的特征数量和用户响应。
> 3. 定义了UF_UI_feat_sel_type_t类型的变量all，设置为UF_UI_FEAT_SEL_TYPE_BROWSEABLE，表示只显示可浏览的特征。
> 4. 调用UF_UI_select_feature函数，传入提示字符串、特征类型、特征数量指针、特征数组指针和用户响应指针。
> 5. 判断用户响应，如果是UF_UI_OK，则表示选择成功，返回特征数量；否则返回0。
> 6. 整个函数的目的是获取用户选择的特征，并返回特征数量。
>
> 总的来说，这段代码实现了在NX中调用UF_UI_select_feature函数，以交互方式让用户选择特征的功能。
>
