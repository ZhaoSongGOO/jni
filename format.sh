#!/bin/bash
# Copyright (c) 2025 zhaosonggo

FORMATTERS=(
  "rs:rustfmt"
  "cpp:clang-format -i"
  "cc:clang-format -i"
  "c:clang-format -i"
  "h:clang-format -i"
  "hpp:clang-format -i"
  "gn:gn format"
  "gni:gn format"
  "py:black"
  "java:clang-format -i"
)

DEFAULT_EXTENSIONS=()
for item in "${FORMATTERS[@]}"; do
  DEFAULT_EXTENSIONS+=("${item%%:*}")
done

function show_help() {
  echo "Usage: $0 [options]"
  echo "Options:"
  echo "  -d, --dir <directory>  Specify the directory to traverse (default: .)"
  echo "  -e, --ext <extensions> Specify file extensions to format (comma-separated, e.g., rs,cpp)"
  echo "  -h, --help             Show this help message"
  exit 0
}

function parse_args() {
  while [[ $# -gt 0 ]]; do
    case "$1" in
      -d|--dir)
        DIR="$2"
        shift 2
        ;;
      -e|--ext)
        IFS=',' read -r -a EXTENSIONS <<< "$2"
        shift 2
        ;;
      -h|--help)
        show_help
        ;;
      *)
        echo "Unknown option: $1"
        show_help
        ;;
    esac
  done
}

function is_ignored() {
  local file="$1"
  if [ -f ".formatignore" ]; then
    while IFS= read -r pattern || [[ -n "$pattern" ]]; do
      if [[ -z "$pattern" || "$pattern" == \#* ]]; then
        continue
      fi
      if [[ "$file" == *"$pattern"* ]]; then
        return 0
      fi
    done < ".formatignore"
  fi
  return 1
}

function get_formatter() {
  local ext="$1"
  for item in "${FORMATTERS[@]}"; do
    if [[ "$item" == "$ext:"* ]]; then
      echo "${item#*:}"
      return
    fi
  done
  echo ""
}

function traverse() {
  local dir="$1"
  for file in "$dir"/*; do
    if [ -d "$file" ]; then
      traverse "$file"
    else
      local ext="${file##*.}"
      if [[ " ${EXTENSIONS[@]} " =~ " ${ext} " ]]; then
        if ! is_ignored "$file"; then
          check_files+=("$file")
        fi
      fi
    fi
  done
}

function main() {
  DIR="."
  EXTENSIONS=("${DEFAULT_EXTENSIONS[@]}")

  parse_args "$@"

  if [ ! -d "$DIR" ]; then
    echo "Error: Directory $DIR does not exist."
    exit 1
  fi

  declare -a check_files=()
  traverse "$DIR"

  for file in "${check_files[@]}"; do
    local ext="${file##*.}"
    local formatter=$(get_formatter "$ext")
    if [ -n "$formatter" ]; then
      echo "Formatting $file with $formatter"
      eval "$formatter \"$file\""
    else
      echo "No formatter found for .$ext files: $file"
    fi
  done
}

main "$@"