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

#include "mycss/selectors/state.h"

bool mycss_selectors_state_token_all(mycss_result_t* result, mycss_token_t* token)
{
    mycss_selectors_t *selectors = result->selectors;
    return selectors->state(result, selectors, selectors->selector, token);
}

bool mycss_selectors_state_token_skip_whitespace(mycss_result_t* result, mycss_token_t* token)
{
    if(token->type != MyCSS_TOKEN_TYPE_WHITESPACE) {
        mycss_selectors_t *selectors = result->selectors;
        return selectors->state(result, selectors, selectors->selector, token);
    }
    
    return true;
}

void mycss_selectors_state_end(mycss_result_t* result)
{
    mycss_selectors_t* selectors = result->selectors;
    mycss_selectors_entry_t* selector = selectors->selector;
    
    if(selector == NULL)
        return;
    
    if(selector->type == MyCSS_SELECTORS_TYPE_ATTRIBUTE ||
       selector->type == MyCSS_SELECTORS_TYPE_FUNCTION  ||
       (selector->key == NULL && selector->value == NULL))
    {
        mycss_selectors_entry_clean(selector);
    }
    else {
        mycss_selectors_parser_selector_end(result, selectors, selector, NULL);
    }
}

/* Generated by utils/selectors.pl and my hands edit (HAND_EDIT_BEGIN and HAND_EDIT_END) */

/* <combinator> */
bool mycss_selectors_state_combinator(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_COLUMN: {
            mycss_selectors_parser_selector_combinator_column(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_column")
            result->parser = selectors->switch_parser;
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            if(*token->data == '+') {
                mycss_selectors_parser_selector_combinator_plus(result, selectors, selector, token);
                MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_plus")
                result->parser = selectors->switch_parser;
            }
            else if(*token->data == '>') {
                mycss_selectors_parser_selector_combinator_greater_than(result, selectors, selector, token);
                MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_greater_than")
                selectors->state = mycss_selectors_state_combinator_greater_than;
                
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
            }
            else if(*token->data == '~') {
                mycss_selectors_parser_selector_combinator_tilde(result, selectors, selector, token);
                MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_tilde")
                result->parser = selectors->switch_parser;
            }
            else {
                mycss_selectors_parser_expectations_error(result, selectors, selector, token);
                result->parser = selectors->switch_parser;
                return false;
            }
            break;
        }
        default: {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_state_combinator_greater_than(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_DELIM) {
        if(*token->data == '>') {
            mycss_selectors_parser_selector_combinator_greater_than(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_greater_than_greater_than")
            result->parser = selectors->switch_parser;
        }
        else {
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    else {
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

/* <simple-selector> */
bool mycss_selectors_state_simple_selector(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_IDENT: {
            mycss_selectors_parser_selector_ident_type(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_ident")
            
            selectors->state = mycss_selectors_state_simple_selector_ident;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            if(*token->data == '*') {
                mycss_selectors_parser_selector_ident_type(result, selectors, selector, token);
                MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_asterisk")
                result->parser = selectors->switch_parser;
            }
            else if(*token->data == '.') {
                selectors->state = mycss_selectors_state_simple_selector_full_stop;
                
                if(result->parser != mycss_selectors_state_token_all)
                    result->parser = mycss_selectors_state_token_all;
            }
            else if(*token->data == '|') {
                mycss_selectors_parser_selector_namespace(result, selectors, selector, token);
                selectors->state = mycss_selectors_state_simple_selector_vertical_bar;
                
                if(result->parser != mycss_selectors_state_token_all)
                    result->parser = mycss_selectors_state_token_all;
            }
            else {
                result->parser = selectors->switch_parser;
                return false;
            }
            break;
        }
        case MyCSS_TOKEN_TYPE_COLON: {
            selectors->state = mycss_selectors_state_simple_selector_colon;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            break;
        }
        case MyCSS_TOKEN_TYPE_LEFT_SQUARE_BRACKET: {
            selectors->state = mycss_selectors_state_simple_selector_left_bracket;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_HASH: {
            mycss_selectors_parser_selector_id(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_hash")
            result->parser = selectors->switch_parser;
            break;
        }
        default: {
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_left_bracket(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_ident_attr(result, selectors, selector, token);
        selectors->state = mycss_selectors_state_simple_selector_left_bracket_ident;
        
        if(result->parser != mycss_selectors_state_token_all)
            result->parser = mycss_selectors_state_token_all;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_DELIM) {
        if(*token->data == '|') {
            mycss_selectors_parser_selector_ident_attr(result, selectors, selector, token);
            mycss_selectors_parser_selector_namespace(result, selectors, selectors->selector, token);
            selectors->state = mycss_selectors_state_simple_selector_left_bracket_vertical_bar;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
        }
        else {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_after_namespace(result, selectors, selector, token);
        selectors->state = mycss_selectors_state_shared_after_wq_name_attr;
        
        if(result->parser != mycss_selectors_state_token_skip_whitespace)
            result->parser = mycss_selectors_state_token_skip_whitespace;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_left_bracket_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            selectors->state = mycss_selectors_state_shared_after_wq_name_attr;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_INCLUDE_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_INCLUDE;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_DASH_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_DASH;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_PREFIX_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_PREFIX;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_SUFFIX_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_SUFFIX;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_SUBSTRING_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_SUBSTRING;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            if(*token->data == '=') {
                if(selector->value == NULL)
                    selector->value = mycss_selectors_value_attribute_create(result, true);
                
                mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_EQUAL;
                selectors->state = mycss_selectors_state_shared_after_attr_matcher;
                
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
            }
            else if(*token->data == '|') {
                mycss_selectors_parser_selector_namespace(result, selectors, selector, token);
                selectors->state = mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar;
                
                if(result->parser != mycss_selectors_state_token_all)
                    result->parser = mycss_selectors_state_token_all;
            }
            else {
                mycss_selectors_parser_expectations_error(result, selectors, selector, token);
                result->parser = selectors->switch_parser;
                return false;
            }
            break;
        }
        case MyCSS_TOKEN_TYPE_RIGHT_SQUARE_BRACKET: {
            mycss_selectors_parser_selector_end(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_left_bracket_ident_right_bracket")
            result->parser = selectors->switch_parser;
            break;
        }
        default: {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_after_namespace(result, selectors, selector, token);
        selectors->state = mycss_selectors_state_shared_after_wq_name_attr;
        
        if(result->parser != mycss_selectors_state_token_skip_whitespace)
            result->parser = mycss_selectors_state_token_skip_whitespace;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_colon(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_IDENT: {
            mycss_selectors_parser_selector_pseudo_class(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_colon_ident")
            result->parser = selectors->switch_parser;
            break;
        }
        case MyCSS_TOKEN_TYPE_COLON: {
            selectors->state = mycss_selectors_state_simple_selector_colon_colon;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            break;
        }
        case MyCSS_TOKEN_TYPE_FUNCTION: {
            mycss_selectors_parser_selector_function(result, selectors, selector, token);
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            break;
        }
        default: {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_colon_colon(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_pseudo_class(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_colon_colon_ident")
        result->parser = selectors->switch_parser;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_FUNCTION) {
        mycss_selectors_parser_selector_function(result, selectors, selector, token);
        
        if(result->parser != mycss_selectors_state_token_all)
            result->parser = mycss_selectors_state_token_all;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_colon_colon_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_parser_selector_function_end(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_colon_colon_function_right_parenthesis")
        result->parser = selectors->switch_parser;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_colon_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_parser_selector_function_end(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_colon_function_right_parenthesis")
        result->parser = selectors->switch_parser;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_full_stop(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_class(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_full_stop_ident")
        result->parser = selectors->switch_parser;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_after_namespace(result, selectors, selector, token);
        mycss_selectors_parser_selector_end(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_vertical_bar_ident")
        result->parser = selectors->switch_parser;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_DELIM) {
        if(*token->data == '*') {
            mycss_selectors_parser_selector_after_namespace(result, selectors, selector, token);
            mycss_selectors_parser_selector_end(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_vertical_bar_asterisk")
            result->parser = selectors->switch_parser;
        }
        else {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_DELIM) {
        if(*token->data == '|') {
            mycss_selectors_parser_selector_namespace(result, selectors, selector, token);
            selectors->state = mycss_selectors_state_simple_selector_ident_vertical_bar;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
        }
        else {
            mycss_selectors_parser_selector_end(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    else {
        mycss_selectors_parser_selector_end(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_simple_selector_ident_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_after_namespace(result, selectors, selector, token);
        mycss_selectors_parser_selector_end(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_ident_vertical_bar_ident")
        result->parser = selectors->switch_parser;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_DELIM) {
        if(*token->data == '*') {
            mycss_selectors_parser_selector_after_namespace(result, selectors, selector, token);
            mycss_selectors_parser_selector_end(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_ident_vertical_bar_asterisk")
            result->parser = selectors->switch_parser;
        }
        else {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_shared_after_wq_name_attr(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_INCLUDE_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_INCLUDE;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_DASH_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_DASH;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_PREFIX_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_PREFIX;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_SUFFIX_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_SUFFIX;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_SUBSTRING_MATCH: {
            if(selector->value == NULL)
                selector->value = mycss_selectors_value_attribute_create(result, true);
            
            mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_SUBSTRING;
            selectors->state = mycss_selectors_state_shared_after_attr_matcher;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            if(*token->data == '=') {
                if(selector->value == NULL)
                    selector->value = mycss_selectors_value_attribute_create(result, true);
                
                mycss_selector_value_attribute(selector->value)->match = MyCSS_SELECTORS_MATCH_EQUAL;
                selectors->state = mycss_selectors_state_shared_after_attr_matcher;
                
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
            }
            else {
                mycss_selectors_parser_expectations_error(result, selectors, selector, token);
                result->parser = selectors->switch_parser;
                return false;
            }
            break;
        }
        case MyCSS_TOKEN_TYPE_RIGHT_SQUARE_BRACKET: {
            mycss_selectors_parser_selector_end(result, selectors, selector, token);
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_shared_after_wq_name_attr_right_bracket")
            result->parser = selectors->switch_parser;
            break;
        }
        default: {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_state_shared_after_attr_matcher(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_parser_selector_value(result, selectors, selector, token);
        selectors->state = mycss_selectors_state_shared_after_attribute_value;
        
        if(result->parser != mycss_selectors_state_token_skip_whitespace)
            result->parser = mycss_selectors_state_token_skip_whitespace;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_STRING) {
        mycss_selectors_parser_selector_value(result, selectors, selector, token);
        selectors->state = mycss_selectors_state_shared_after_attribute_value;
        
        if(result->parser != mycss_selectors_state_token_skip_whitespace)
            result->parser = mycss_selectors_state_token_skip_whitespace;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_shared_after_attribute_value(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        if(*token->data == 'i') {
            mycss_selectors_parser_selector_modifier(result, selectors, selector, token);
            selectors->state = mycss_selectors_state_shared_after_attr_modifier;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
        }
        else {
            mycss_selectors_parser_expectations_error(result, selectors, selector, token);
            result->parser = selectors->switch_parser;
            return false;
        }
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_SQUARE_BRACKET) {
        mycss_selectors_parser_selector_end(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_shared_after_attribute_value_right_bracket")
        result->parser = selectors->switch_parser;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_state_shared_after_attr_modifier(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_SQUARE_BRACKET) {
        mycss_selectors_parser_selector_end(result, selectors, selector, token);
        MyCSS_DEBUG_MESSAGE("mycss_selectors_state_shared_after_attr_modifier_right_bracket")
        result->parser = selectors->switch_parser;
    }
    else {
        mycss_selectors_parser_expectations_error(result, selectors, selector, token);
        result->parser = selectors->switch_parser;
        return false;
    }
    
    return true;
}


