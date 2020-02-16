# coding: utf-8

import asyncio
import time

from websockets import serve, connect, WebSocketServerProtocol
from threading import Thread

# import logging
# logger = logging.getLogger('websockets')
# logger.setLevel(logging.DEBUG)
# logger.addHandler(logging.StreamHandler())

event_loop = asyncio.get_event_loop()


def websocket_thread_func(loop, ip, port):
    print('websocket_thread_func thread running')
    asyncio.set_event_loop(loop)
    loop.run_until_complete(serve(echo, ip, port))
    loop.run_forever()
    print('websocket_thread_func thread exiting')


def start_websocket_thread(ip, port):
    print('inside start_websocket_thread()')
    t = Thread(target=websocket_thread_func, args=(event_loop, ip, port))
    t.start()


def stop_websocket_thread(ip, port):
    temp_loop = asyncio.new_event_loop()
    stop_task = temp_loop.create_task(stop_websocket_select(ip, port))
    temp_loop.run_until_complete(stop_task)


async def stop_websocket_select(ip, port):
    print("ws://%s:%d" % (ip, port))
    async with connect("ws://%s:%d" % (ip, port)) as websocket:
        # event_loop.stop()
        start = time.time()
        print('------------------ start:%f' % start)
        # await websocket.send('close self')
        # print('------------------ middle:%f' % time.time())
        await websocket.close(1000)
        event_loop.stop()
        end = time.time()
        print('====================== end:%f cost:%f' % (end, end-start))


async def echo(websocket: WebSocketServerProtocol, path):
    async for message in websocket:
        print("%s  %s   <----- from %s:%d" % (message, path, websocket.remote_address[0], websocket.remote_address[1]))
        await websocket.send(message)
