import { configureStore } from '@reduxjs/toolkit';
import roomSlice, { RoomState } from './slices/room';
import deviceSlice, { DeviceState } from './slices/device';
import streamSlice, { StreamState } from './slices/stream';
import statsSlice, { StatsState } from './slices/streamStats';
import { roomQuery } from '@/app/roomQuery';

export interface RootState {
  room: RoomState;
  device: DeviceState;
  stream: StreamState;
  streamStats: StatsState;
}

const store = configureStore({
  reducer: {
    device: deviceSlice,
    stream: streamSlice,
    streamStats: statsSlice,
    room: roomSlice,
    [roomQuery.reducerPath]: roomQuery.reducer,
  },
  middleware: (getDefaultMiddleware) =>
    getDefaultMiddleware({
      serializableCheck: false,
    }).concat(roomQuery.middleware),
});

export default store;
