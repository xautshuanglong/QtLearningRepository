#pragma once

template<class T>
void print_queue(T q)
{
    std::stringstream ss;
    while (!q.empty())
    {
        ss << q.top() << ' ';
        q.pop();
    }
    LogUtil::Debug(CODE_LOCATION, "std::priority_queue : %s", ss.str().c_str());
}

template<class T>
void print_list(std::list<T> list)
{
    std::stringstream ss;
    for (T element : list)
    {
        ss << element << ' ';
    }
    LogUtil::Debug(CODE_LOCATION, "std::list : %s", ss.str().c_str());
}
