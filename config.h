constexpr auto SRU_SCHEMA = R"({
    "definitions": {},
    "$schema": "http://json-schema.org/draft-07/schema#",
    "$id": "http://example.com/root.json",
    "type": "object",
    "title": "The Root Schema",
    "additionalProperties": false,
    "required": [
        "date_provider",
        "base_path",
        "base_size",
        "pages",
        "anchors",
        "objects",
        "calcs"
    ],
    "properties": {
        "date_provider": {
            "$id": "#/properties/date_provider",
            "type": "integer",
            "title": "The date_provider Schema"
        },
        "base_path": {
            "$id": "#/properties/base_path",
            "type": "string",
            "title": "The base_size Schema"
        },
        "base_size": {
            "$id": "#/properties/base_size",
            "type": "integer",
            "title": "The base_size Schema"
        },
        "pages": {
            "$id": "#/properties/pages",
            "type": "array",
            "title": "The pages Schema",
            "items": {
                "$id": "#/properties/pages/items",
                "type": "object",
                "title": "The Anchor calcs Schema",
                "additionalProperties": false,
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
                        "enum": ["append", "delete", "none"]
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
                            "title": "The Items Schema",
                            "minimum": 0
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
                "additionalProperties": false,
                "required": [
                    "id",
                    "x",
                    "y",
                    "anchor_name",
                    "content_id",
                    "sub_groups"
                ],
                "properties": {
                    "id": {
                        "$id": "#/properties/anchors/items/properties/id",
                        "type": "integer",
                        "title": "The id Schema",
                        "minimum": 0
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
            "title": "The objects Schema",
            "items": {
                "$id": "#/properties/objects/properties/items",
                "type": "object",
                "title": "The Items Schema",
                "additionalProperties": false,
                "required": [
                    "id",
                    "object_name",
                    "calcs"
                ],
                "properties": {
                    "id": {
                        "$id": "#/properties/objects/properties/items/properties/id",
                        "type": "integer",
                        "title": "The id Schema",
                        "minimum": 0
                    },
                    "object_name": {
                        "$id": "#/properties/objects/properties/items/properties/object_name",
                        "type": "string",
                        "title": "The object_name Schema"
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
                    "anchor_margin_x": {
                        "$id": "#/properties/objects/properties/items/properties/anchor_margin_x",
                        "type": "integer",
                        "title": "The anchor_margin_x Schema",
                        "minimum": 0
                    },
                    "anchor_margin_y": {
                        "$id": "#/properties/objects/properties/items/properties/anchor_margin_y",
                        "type": "integer",
                        "title": "The anchor_margin_y Schema",
                        "minimum": 0
                    },
                    "object_count": {
                        "$id": "#/properties/objects/properties/items/properties/object_count",
                        "type": "integer",
                        "title": "The object_count Schema",
                        "minimum": 0
                    },
                    "y_object_spacing": {
                        "$id": "#/properties/objects/properties/items/properties/y_object_spacing",
                        "type": "number",
                        "title": "The y_object_spacing Schema"
                    },
                    "text_justify": {
                        "$id": "#/properties/calcs/properties/items/properties/text_justify",
                        "type": "number",
                        "title": "The text_justify Schema"
                    },
                    "sticky_id": {
                        "$id": "#/properties/objects/properties/items/properties/sticky_id",
                        "type": "integer",
                        "title": "The sticky_id Schema",
                        "minimum": 0
                    },
                    "calcs": {
                        "$id": "#/properties/objects/properties/items/properties/calcs",
                        "type": "array",
                        "title": "The calcs Schema",
                        "items": {
                            "$id": "#/properties/objects/properties/items/properties/calcs/items",
                            "type": "integer",
                            "title": "The Items Schema",
                            "minimum": 0
                        }
                    }
                }
            }
        },
        "calcs": {
            "$id": "#/properties/calcs",
            "type": "array",
            "title": "The calcs Schema",
            "items": {
                "$id": "#/properties/calcs/properties/items",
                "type": "object",
                "title": "The Items Schema",
                "additionalProperties": false,
                "required": [
                    "id",
                    "calc_name",
                    "calc_mode"
                ],
                "properties": {
                    "id": {
                        "$id": "#/properties/calcs/properties/items/properties/id",
                        "type": "integer",
                        "title": "The id Schema",
                        "minimum": 0
                    },
                    "calc_name": {
                        "$id": "#/properties/calcs/properties/items/properties/calc_name",
                        "type": "string",
                        "title": "The calc_name Schema"
                    },
                    "maximum_values": {
                        "$id": "#/properties/calcs/properties/items/properties/maximum_values",
                        "type": "integer",
                        "title": "The maximum_values Schema",
                        "minimum": 0
                    },
                    "round_cut_off": {
                        "$id": "#/properties/calcs/properties/items/properties/round_cut_off",
                        "type": "number",
                        "title": "The round_cut_off Schema"
                    },
                    "decimal_points": {
                        "$id": "#/properties/calcs/properties/items/properties/decimal_points",
                        "type": "integer",
                        "title": "The decimal_points Schema"
                    },
                    "calc_mode": {
                        "$id": "#/properties/calcs/properties/items/properties/calc_mode",
                        "type": "string",
                        "title": "The calc_mode Schema",
                        "enum": [
                            "SORT",
                            "AVRG",
                            "SUM",
                            "USER_INPUT",
                            "TABLE",
                            "NUMBERING",
                            "FORWARD",
                            ""
                        ]
                    },
                    "avrg_self": {
                        "$id": "#/properties/calcs/properties/items/properties/avrg_self",
                        "type": "boolean",
                        "title": "The avrg_self Schema"
                    },
                    "avrg_source_id": {
                        "$id": "#/properties/calcs/properties/items/properties/avrg_source_id",
                        "type": "array",
                        "title": "The avrg_source_id Schema",
                        "items": {
                            "$id": "#/properties/calcs/properties/items/properties/avrg_source_id/items",
                            "type": "integer",
                            "title": "The Items Schema",
                            "minimum": 0
                        },
                        "minItems": 2,
                        "maxItems": 2
                    },
                    "avrg_base_id": {
                        "$id": "#/properties/calcs/properties/items/properties/avrg_base_id",
                        "type": "array",
                        "title": "The avrg_base_id Schema",
                        "items": {
                            "$id": "#/properties/calcs/properties/items/properties/avrg_base_id/items",
                            "type": "integer",
                            "title": "The Items Schema",
                            "minimum": 0
                        },
                        "minItems": 2,
                        "maxItems": 2
                    },
                    "avrg_multiplier": {
                        "$id": "#/properties/calcs/properties/items/properties/avrg_multiplier",
                        "type": "integer",
                        "title": "The avrg_multiplier Schema"
                    },
                    "overflow_threshold": {
                        "$id": "#/properties/calcs/properties/items/properties/overflow_threshold",
                        "type": "number",
                        "title": "The overflow_threshold Schema"
                    },
                    "zfill": {
                        "$id": "#/properties/calcs/properties/items/properties/zfill",
                        "type": "integer",
                        "title": "The zfill Schema"
                    },
                    "sort_settings": {
                        "$id": "#/properties/calcs/properties/items/properties/sort_settings",
                        "type": "array",
                        "title": "The sort_settings Schema",
                        "items": {
                            "$id": "#/properties/calcs/properties/items/properties/sort_settings/items",
                            "type": "boolean",
                            "title": "The Items Schema"
                        }
                    },
                    "re_extract_order": {
                        "$id": "#/properties/calcs/properties/items/properties/re_extract_order",
                        "type": "array",
                        "title": "The re_extract_order Schema",
                        "items": {
                            "$id": "#/properties/calcs/properties/items/properties/re_extract_order/items",
                            "type": "integer",
                            "title": "The Items Schema",
                            "minimum": 0
                        }
                    },
                    "regex": {
                        "$id": "#/properties/calcs/properties/items/properties/regex",
                        "type": "string",
                        "title": "The regex Schema"
                    },
                    "margin_x": {
                        "$id": "#/properties/calcs/properties/items/properties/margin_x",
                        "type": "number",
                        "title": "The margin_x Schema"
                    },
                    "margin_y": {
                        "$id": "#/properties/calcs/properties/items/properties/margin_y",
                        "type": "number",
                        "title": "The margin_y Schema"
                    },
                    "anchor_margin_x": {
                        "$id": "#/properties/calcs/properties/items/properties/anchor_margin_x",
                        "type": "integer",
                        "title": "The anchor_margin_x Schema",
                        "minimum": 0
                    },
                    "anchor_margin_y": {
                        "$id": "#/properties/calcs/properties/items/properties/anchor_margin_y",
                        "type": "integer",
                        "title": "The anchor_margin_y Schema",
                        "minimum": 0
                    },
                    "object_count": {
                        "$id": "#/properties/calcs/properties/items/properties/object_count",
                        "type": "integer",
                        "title": "The object_count Schema",
                        "minimum": 0
                    },
                    "sticky_id": {
                        "$id": "#/properties/calcs/properties/items/properties/sticky_id",
                        "type": "integer",
                        "title": "The sticky_id Schema",
                        "minimum": 0
                    }
                },
                "allOf": [
                    {
                        "if": {
                            "properties": {
                                "calc_mode": {
                                    "const": "AVRG"
                                }
                            }
                        },
                        "then": {
                            "if": {
                                "properties": {
                                    "avrg_self": {
                                        "const": false
                                    }
                                }
                            },
                            "then": {
                                "required": [
                                    "avrg_base_id",
                                    "avrg_source_id"
                                ]
                            },
                            "else": {
                                "required": [
                                    "avrg_self"
                                ]
                            },
                            "required": [
                                "avrg_multiplier",
                                "round_cut_off",
                                "decimal_points",
                                "re_extract_order",
                                "regex"
                            ]
                        }
                    },
                    {
                        "if": {
                            "properties": {
                                "calc_mode": {
                                    "const": "SORT"
                                }
                            }
                        },
                        "then": {
                            "required": [
                                "maximum_values",
                                "sort_settings",
                                "re_extract_order",
                                "regex"
                            ]
                        }
                    },
                    {
                        "if": {
                            "properties": {
                                "calc_mode": {
                                    "const": "SUM"
                                }
                            }
                        },
                        "then": {
                            "required": [
                                "round_cut_off",
                                "decimal_points",
                                "overflow_threshold",
                                "re_extract_order",
                                "regex"
                            ]
                        }
                    }
                ]
            }
        }
    }
})";