#pragma once

#define JENO_NON_COPYABLE(className)					\
	className(const className&) = delete;				\
	className& operator=(const className&) = delete;

#define JENO_NON_MOVE_AND_COPYABLE(className)					\
	className(const className&) = delete;						\
	className& operator=(const className&) = delete;			\
	className(className&& other) noexcept = delete;				\
	className& operator=(className&& other) noexcept = delete;

#define JENO_DEFAULT_MOVABLE(className)					\
	className(className&&) noexcept = default;				\
	className& operator=(className&&) noexcept = default;

#define JENO_DECLARE_MOVE_ONLY(className)				\
	className(const className&) = delete;				\
	className& operator=(const className&) = delete;	\
	className(className&& other) noexcept;				\
	className& operator=(className&& other) noexcept;

#define JENO_MOVE_ONLY(className)							\
	className(const className&) = delete;					\
	className& operator=(const className&) = delete;		\
	className(className&&) noexcept = default;				\
	className& operator=(className&&) noexcept = default;