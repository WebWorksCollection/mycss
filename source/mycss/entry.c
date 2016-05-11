/*
 Copyright (C) 2016 Alexander Borisov
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 
 Author: lex.borisov@gmail.com (Alexander Borisov)
*/

#include "mycss/entry.h"


mycss_entry_t * mycss_entry_create(void)
{
    return (mycss_entry_t*)mycalloc(1, sizeof(mycss_entry_t));
}

mycss_status_t mycss_entry_init(mycss_t* mycss, mycss_entry_t* entry)
{
    entry->mycss = mycss;
    
    mcobject_async_status_t mcstatus;
    entry->incoming_buffer_id = mcobject_async_node_add(mycss->async_incoming_buffer, &mcstatus);
    
    if(mcstatus)
        return MyCSS_STATUS_ERROR_ENTRY_INCOMING_BUFFER_INIT;
    
    // init for token objects
    entry->mcasync_token = mcobject_async_create();
    if(entry->mcasync_token == NULL)
        return MyCSS_STATUS_ERROR_MEMORY_ALLOCATION;
    
    mcstatus = mcobject_async_init(entry->mcasync_token, 32, 1024, sizeof(mycss_token_t));
    if(mcstatus != MCOBJECT_ASYNC_STATUS_OK)
        return MyCSS_STATUS_ERROR_ENTRY_TOKEN_INCOMING_BUFFER_INIT;
    
    // add new node for a tokens object
    entry->token_id = mcobject_async_node_add(entry->mcasync_token, &mcstatus);
    
    if(mcstatus)
        return MyCSS_STATUS_ERROR_ENTRY_TOKEN_NODE_ADD;
    
    entry->mchar = mchar_async_create(128, (4096 * 5));
    entry->mchar_node_id = mchar_async_node_add(entry->mchar);
    
    entry->token_ready_callback = mycss_parser_token_ready_callback_function;
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_entry_clean_all(mycss_entry_t* entry)
{
    mcobject_async_node_clean(entry->mycss->async_incoming_buffer, entry->incoming_buffer_id);
    mcobject_async_clean(entry->mcasync_token);
    mchar_async_node_clean(entry->mchar, entry->mchar_node_id);
    
    entry->token                = NULL;
    entry->state                = MyCSS_TOKENIZER_STATE_DATA;
    entry->state_back           = MyCSS_TOKENIZER_STATE_DATA;
    //entry->token_ready_callback = mycss_parser_token_ready_callback_function;
    entry->first_buffer         = NULL;
    entry->current_buffer       = NULL;
    entry->token_counter        = 0;
    entry->help_counter         = 0;
    entry->type                 = MyCSS_ENTRY_TYPE_CLEAN;
    
    mcobject_async_status_t mcstatus;
    entry->token_id = mcobject_async_node_add(entry->mcasync_token, &mcstatus);
    
    if(mcstatus)
        return MyCSS_STATUS_ERROR_ENTRY_TOKEN_NODE_ADD;
    
    return MyCSS_STATUS_OK;
}

mycss_entry_t * mycss_entry_destroy(mycss_entry_t* entry, bool self_destroy)
{
    if(entry == NULL)
        return NULL;
    
    mcobject_async_node_delete(entry->mycss->async_incoming_buffer, entry->incoming_buffer_id);
    
    entry->mcasync_token = mcobject_async_destroy(entry->mcasync_token, 1);
    entry->mchar         = mchar_async_destroy(entry->mchar, 1);
    
    if(self_destroy) {
        myfree(entry);
        return NULL;
    }
    
    return entry;
}

mycss_token_ready_callback_f mycss_entry_token_ready_callback(mycss_entry_t* entry, mycss_token_ready_callback_f callback_f)
{
    if(callback_f)
        entry->token_ready_callback = callback_f;
    
    return entry->token_ready_callback;
}

size_t mycss_entry_token_count(mycss_entry_t* entry)
{
    return entry->token_counter;
}

myhtml_incoming_buffer_t * mycss_entry_incoming_buffer_current(mycss_entry_t* entry)
{
    return entry->current_buffer;
}

myhtml_incoming_buffer_t * mycss_entry_incoming_buffer_first(mycss_entry_t* entry)
{
    return entry->first_buffer;
}

