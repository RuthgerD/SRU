constexpr auto SRU_SCHEMA = R"({
    "definitions": {},
    "$schema": "http://json-schema.org/draft-07/schema#",
    "$id": "http://example.com/root.json",
    "type": "object",
    "title": "The Root Schema",
    "required": [
        "pages",
        "anchors",
        "objects"
    ],
    "properties": {
        "pages": {
            "$id": "#/properties/pages",
            "type": "array",
            "title": "The pages Schema",
            "items": {
                "$id": "#/properties/pages/items",
                "type": "object",
                "title": "The Anchor Objects Schema",
                "required": [
                    "page_name",
                    "priority",
                    "mutate_in_final",
                    "regex_id",
                    "obj_regex",
                    "groups"
                ],
                "properties": {
                    "page_name": {
                        "$id": "#/properties/pages/items/properties/page_name",
                        "type": "string",
                        "title": "The regex_id Schema",
                        "pattern": "^(.*)$"
                    },
                    "priority": {
                        "$id": "#/properties/pages/items/properties/priority",
                        "type": "integer",
                        "title": "The priority Schema"
                    },
                    "mutate_in_final": {
                        "$id": "#/properties/pages/items/properties/mutate_in_final",
                        "type": "string",
                        "title": "The mutate_in_final Schema",
                        "pattern": "^(append|delete|none)$"
                    },
                    "regex_id": {
                        "$id": "#/properties/pages/items/properties/regex_id",
                        "type": "string",
                        "title": "The regex_id Schema",
                        "pattern": "^(.*)$"
                    },
                    "obj_regex": {
                        "$id": "#/properties/pages/items/properties/obj_regex",
                        "type": "string",
                        "title": "The obj_regex Schema",
                        "pattern": "^(.*)$"
                    },
                    "groups": {
                        "$id": "#/properties/pages/items/properties/groups",
                        "type": "array",
                        "title": "The groups Schema",
                        "items": {
                            "$id": "#/properties/pages/items/properties/groups/items",
                            "type": "integer",
                            "title": "The Items Schema"
                        }
                    }
                }
            }
        },
        "anchors": {
            "$id": "#/properties/anchors",
            "type": "array",
            "title": "The anchors Schema",
            "items": {
                "$id": "#/properties/anchors/items",
                "type": "object",
                "title": "The Anchor Schema",
                "required": [
                    "id",
                    "is_virtual",
                    "x",
                    "y",
                    "anchor_name",
                    "content_id",
                    "content_",
                    "content_alt",
                    "save_anchor",
                    "sub_groups"
                ],
                "properties": {
                    "id": {
                        "$id": "#/properties/anchors/items/properties/id",
                        "type": "integer",
                        "title": "The id Schema"
                    },
                    "is_virtual": {
                        "$id": "#/properties/anchors/items/properties/is_virtual",
                        "type": "boolean",
                        "title": "The is_virtual Schema"
                    },
                    "x": {
                        "$id": "#/properties/anchors/items/properties/x",
                        "type": "number",
                        "title": "The x Schema"
                    },
                    "y": {
                        "$id": "#/properties/anchors/items/properties/y",
                        "type": "number",
                        "title": "The y Schema"
                    },
                    "anchor_name": {
                        "$id": "#/properties/anchors/properties/items/properties/anchor_name",
                        "type": "string",
                        "title": "The anchor_name Schema",
                        "pattern": "^(.*)$"
                    },
                    "content_id": {
                        "$id": "#/properties/anchors/items/properties/content_id",
                        "type": "string",
                        "title": "The Content_id Schema",
                        "pattern": "^(.*)$"
                    },
                    "content_": {
                        "$id": "#/properties/anchors/items/properties/content_",
                        "type": "string",
                        "title": "The Content_ Schema",
                        "pattern": "^(.*)$"
                    },
                    "content_alt": {
                        "$id": "#/properties/anchors/items/properties/content_alt",
                        "type": "string",
                        "title": "The Content_alt Schema",
                        "pattern": "^(.*)$"
                    },
                    "save_anchor": {
                        "$id": "#/properties/anchors/items/properties/save_anchor",
                        "type": "boolean",
                        "title": "The Save_anchor Schema"
                    },
                    "sub_groups": {
                        "$id": "#/properties/anchors/items/properties/sub_groups",
                        "type": "array",
                        "title": "The Sub_groups Schema",
                        "items": {
                            "$id": "#/properties/anchors/items/properties/sub_groups/items",
                            "type": "integer",
                            "title": "The Items Schema"
                        }
                    }
                }
            }
        },
        "objects": {
            "$id": "#/properties/objects",
            "type": "array",
            "title": "The Objects Schema",
            "items": {
                "$id": "#/properties/objects/properties/items",
                "type": "object",
                "title": "The Items Schema",
                "required": [
                    "id",
                    "object_name",
                    "text_justify",
                    "maximum_values",
                    "y_object_spacing",
                    "round_cut_off",
                    "decimal_points",
                    "calc_modes",
                    "avrg_self",
                    "avrg_source_id",
                    "avrg_base_id",
                    "avrg_multiplier",
                    "overflow_threshold",
                    "sort_settings",
                    "re_extract_order",
                    "regexs",
                    "margin_x",
                    "margin_y",
                    "object_count",
                    "sticky_id"
                ],
                "properties": {
                    "id": {
                        "$id": "#/properties/objects/properties/items/properties/id",
                        "type": "integer",
                        "title": "The id Schema"
                    },
                    "object_name": {
                        "$id": "#/properties/objects/properties/items/properties/object_name",
                        "type": "string",
                        "title": "The object_name Schema",
                        "pattern": "^(.*)$"
                    },
                    "text_justify": {
                        "$id": "#/properties/objects/properties/items/properties/text_justify",
                        "type": "number",
                        "title": "The text_justify Schema"
                    },
                    "maximum_values": {
                        "$id": "#/properties/objects/properties/items/properties/maximum_values",
                        "type": "integer",
                        "title": "The maximum_values Schema"
                    },
                    "y_object_spacing": {
                        "$id": "#/properties/objects/properties/items/properties/y_object_spacing",
                        "type": "number",
                        "title": "The y_object_spacing Schema"
                    },
                    "round_cut_off": {
                        "$id": "#/properties/objects/properties/items/properties/round_cut_off",
                        "type": "number",
                        "title": "The round_cut_off Schema"
                    },
                    "decimal_points": {
                        "$id": "#/properties/objects/properties/items/properties/decimal_points",
                        "type": "integer",
                        "title": "The decimal_points Schema"
                    },
                    "calc_modes": {
                        "$id": "#/properties/objects/properties/items/properties/calc_modes",
                        "type": "array",
                        "title": "The calc_modes Schema",
                        "items": {
                            "$id": "#/properties/objects/properties/items/properties/calc_modes/items",
                            "type": "string",
                            "title": "The Items Schema"
                        }
                    },
                    "avrg_self": {
                        "$id": "#/properties/objects/properties/items/properties/avrg_self",
                        "type": "boolean",
                        "title": "The avrg_self Schema"
                    },
                    "avrg_source_id": {
                        "$id": "#/properties/objects/properties/items/properties/avrg_source_id",
                        "type": "integer",
                        "title": "The avrg_source_id Schema"
                    },
                    "avrg_base_id": {
                        "$id": "#/properties/objects/properties/items/properties/avrg_base_id",
                        "type": "integer",
                        "title": "The avrg_base_id Schema"
                    },
                    "avrg_multiplier": {
                        "$id": "#/properties/objects/properties/items/properties/avrg_multiplier",
                        "type": "integer",
                        "title": "The avrg_multiplier Schema"
                    },
                    "overflow_threshold": {
                        "$id": "#/properties/objects/properties/items/properties/overflow_threshold",
                        "type": "number",
                        "title": "The overflow_threshold Schema"
                    },
                    "sort_settings": {
                        "$id": "#/properties/objects/properties/items/properties/sort_settings",
                        "type": "array",
                        "title": "The sort_settings Schema",
                        "items": {
                            "$id": "#/properties/objects/properties/items/properties/sort_settings/items",
                            "type": "boolean",
                            "title": "The Items Schema"
                        }
                    },
                    "re_extract_order": {
                        "$id": "#/properties/objects/properties/items/properties/re_extract_order",
                        "type": "array",
                        "title": "The re_extract_order Schema",
                        "items": {
                            "$id": "#/properties/objects/properties/items/properties/re_extract_order/items",
                            "type": "integer",
                            "title": "The Items Schema"
                        }
                    },
                    "regexs": {
                        "$id": "#/properties/objects/properties/items/properties/regexs",
                        "type": "array",
                        "title": "The regexs Schema",
                        "items": {
                            "$id": "#/properties/objects/properties/items/properties/regexs/items",
                            "type": "string",
                            "title": "The Items Schema"
                        }
                    },
                    "margin_x": {
                        "$id": "#/properties/objects/properties/items/properties/margin_x",
                        "type": "number",
                        "title": "The margin_x Schema"
                    },
                    "margin_y": {
                        "$id": "#/properties/objects/properties/items/properties/margin_y",
                        "type": "number",
                        "title": "The margin_y Schema"
                    },
                    "object_count": {
                        "$id": "#/properties/objects/properties/items/properties/object_count",
                        "type": "integer",
                        "title": "The object_count Schema"
                    },
                    "sticky_id": {
                        "$id": "#/properties/objects/properties/items/properties/sticky_id",
                        "type": "integer",
                        "title": "The sticky_id Schema"
                    }
                }
            }
        }
    }
})";