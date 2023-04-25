#pragma once

struct SnapshotAttr {
    // thumbnails type
    enum SnapshotType
    {
    kUnkonw = 0,
    // display screen share
    kScreen,
    //  window share
    kWindow
    };
    // thumbnail name
    std::string name;
    SnapshotType type = kUnkonw;
    // share source id
    void* source_id;
    // thumbnail index
    int index = 0;
};