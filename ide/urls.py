from django.urls import path
import ide.apis as api

urlpatterns = [
    path('get_session/',api.get_debuger_session, name="session"),
    path('get_device/', api.get_device, name="device"),
    path('get_memory_info/', api.get_memory_info, name="memory.info"),
    path('read_memory_block/', api.readMemoryBlock, name="memory.block"),
    path('search_memory/', api.searchMemory, name="memory.search"),
    path('get_peripherals/', api.getPeripherals, name="peripherals"),
    path('get_peripheral-by-id/', api.getPeripheralById, name="peripheral_by_id"),
    
    path('command_device/',api.command_device, name="command" ),
]