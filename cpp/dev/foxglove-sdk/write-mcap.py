#!/usr/bin/env python3
import argparse
import inspect
import numpy as np
import foxglove
import json
import time
import datetime
from foxglove import Channel, Schema
from foxglove.channels import LogChannel
from foxglove.schemas import Log, LogLevel

# from foxglove.schemas import (
#     Color,
#     CubePrimitive,
#     Duration,
#     FrameTransform,
#     FrameTransforms,
#     PackedElementField,
#     PackedElementFieldNumericType,
#     PointCloud,
#     Pose,
#     Quaternion,
#     RawImage,
#     SceneEntity,
#     SceneUpdate,
#     Timestamp,
#     Vector3
# )


parser = argparse.ArgumentParser()
parser.add_argument("--path", type=str, default="output.mcap")
args = parser.parse_args()


# log_chan = LogChannel(topic="/log1")

msg_schema = {
    "type": "object",
    "properties": {
        "timestamp": {"type": "number"},
        "x": {"type": "number"},
        "y": {"type": "number"},
        "z": {"type": "number"},
    },
}

msg_chan = Channel(
  topic="/vec",
  message_encoding="json",
  schema=Schema(
    name="vec",
    encoding="jsonschema",
    data=json.dumps(msg_schema).encode("utf-8")
  )
)


def main() -> None:
    # Create a new mcap file at the given path for recording
    try:
      with foxglove.open_mcap(args.path, allow_overwrite=True):
          # for i in range(10):
          # i = 0
          while True:
              time.sleep(0.01)
              # frame = inspect.currentframe()
              # frameinfo = inspect.getframeinfo(frame) if frame else None

              smp = np.random.normal(size=3)

              msg = {
                "timestamp": time.time(),
                "x": smp[0],
                "y": smp[1],
                "z": smp[2]
              }

              msg_chan.log(msg)

              # foxglove.log(
              #     "/log2",
              #     Log(
              #         level=LogLevel.Info,
              #         name="SDK example",
              #         file=frameinfo.filename if frameinfo else None,
              #         line=frameinfo.lineno if frameinfo else None,
              #         message=f"message {i}",
              #     ),
              # )

              # # Or use a typed channel directly to get better type checking
              # log_chan.log(
              #     Log(
              #         level=LogLevel.Info,
              #         name="SDK example",
              #         file=frameinfo.filename if frameinfo else None,
              #         line=frameinfo.lineno if frameinfo else None,
              #         message=f"message {i}",
              #     ),
              # )

              # i += 1
    except KeyboardInterrupt:
        print("ctrl-c ...")


if __name__ == "__main__":
    main()