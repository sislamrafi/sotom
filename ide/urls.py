from django.urls import path
import ide.apis as api

urlpatterns = [
    path('command_device/',api.command_device, name="command" ),
    path('get_session/',api.get_debuger_session, name="session"),
    path('get_device/', api.get_device, name="device"),
    path('get_memory_info/', api.get_memory_info, name="memory.info"),
    path('search_memory/', api.searchMemory, name="memory.search"),
]